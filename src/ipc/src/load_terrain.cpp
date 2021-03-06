/**
 * @file load_terrain.cpp
 * Function definitions for loading terrain from file
*/
#include <iostream>
#include <string>
#include <fstream>
#include "ipc.h"
#include "terrain/terrain.h"
#include "terrain/terrain_element.h"
#include "terrain.pb.h"

/**
 * Depopulates the terrain
 * 
 * Terrain is a grid where each unit (terrain element) consists of position
 * size, terrain type, line of sight for a given player and timestamp denoting
 * last visit by the players
 *
 * @param[in]  TerrainMessage  the IPC::Terrain message object
 * @param[out] TerrainObject   the state::Terrain object 
 */
state::Terrain DepopulateTerrain(const IPC::Terrain& TerrainMessage) {

	std::vector<std::vector<state::TerrainElement> > Grid;

	const int NoOfRows = TerrainMessage.no_of_rows();
	const int SideLength = TerrainMessage.size_of_element();

	/**
	 * Iterates through the TerrainMessage object and fills in the Grid with
	 * position, size and type of terrain
	 */
	for (int i = 0; i < TerrainMessage.row_size(); i++)
	{
		const IPC::Terrain::TerrainRow& RowMessage = TerrainMessage.row(i);
		std::vector<state::TerrainElement> Row;
		for(int j = 0; j < RowMessage.element_size(); j++)
		{

			const IPC::Terrain::TerrainElement& ElementMessage = RowMessage.element(j);
			
			state::TERRAIN_TYPE terrain_type;
			switch(ElementMessage.type())
			{
				case IPC::Terrain::TerrainElement::PLAIN : 
					terrain_type = state::PLAIN;
					break;
				case IPC::Terrain::TerrainElement::FOREST : 
					terrain_type = state::FOREST;
					break;
				case IPC::Terrain::TerrainElement::MOUNTAIN : 
					terrain_type = state::MOUNTAIN;
					break;
			}
			Row.push_back(state::TerrainElement(terrain_type, physics::Vector2D(i * SideLength, j * SideLength), SideLength));
		}
		Grid.push_back(Row);
	}
	state::Terrain TerrainObject(Grid);

	return TerrainObject;
}

namespace ipc {

	/**
	 * Loads the terrain from a file
	 *
	 * @param[in]  filename		the storage file
	 *
	 * @return     Terrain 		the loaded terrain
	 */

	state::Terrain LoadTerrain(std::string filename) {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::Terrain TerrainMessage;

		std::fstream input(filename, std::ios::in | std::ios::binary);

		TerrainMessage.ParseFromIstream(&input);

		return ( DepopulateTerrain(TerrainMessage) );
	}
}
