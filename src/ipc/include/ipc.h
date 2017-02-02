/**
 * @file ipc.h
 * Function declarations for inter process communication methods between
 * renderer and simulator
*/
#ifndef IPC_IPC_H
#define IPC_IPC_H

#include "state.h"
#include "ipc_export.h"
#include <atomic>

namespace ipc {

/**
 * Interrupts from the renderer to handle user interaction
 */
class IPC_EXPORT Interrupts {

private:
		/**
		 * Describe simulation status
		 * True: Play, False: Pause
		 */
		std::atomic<bool> play;

		/**
		 * Describe level number
		 */
		std::atomic<int> level;

		/**
		 * Describe the exit status
		 */
		std::atomic<bool> exit;

		/**
		 * Describe the restart status
		 */
		std::atomic<bool> restart;

public:
		/**
		 * Constructor for Interrupts
		 */
		Interrupts();

		/**
		 * Gets the state of the simulation
		 *
		 * @return     play status
		 */
		bool GetPlayStatus();

		/**
		 * Gets the level number of currently loaded level
		 *
		 * @return     The level number
		 */
		int GetLevelNumber();

		/**
		 * Gets the exit status of the simulator
		 *
		 * @return     The exit status
		 */
		bool GetExitStatus();

		/**
		 * Gets the restart status of the simulator
		 *
		 * @return     The restart status
		 */
		bool GetRestartStatus();

		/**
		 * Sets the state of the simulation
		 */
		void SetPlayStatus(bool play_status);

		/**
		 * Sets the level number of currently loaded level
		 */
		void SetLevelNumber(int current_level);

		/**
		 * Sets the exit status of the simulator
		 */
		void SetExitStatus(bool exit_status);

		/**
		 * Sets the restart status of the simulator
		 */
		void SetRestartStatus(bool restart_status);

};

class IPC_EXPORT Logger {

	std::vector<std::string> logs;
	static Logger *static_instance;

	public:
		std::vector<std::string> GetLogs() {
			return logs;
		}
		void SetLogs(std::string log) {
			logs.push_back(log);
		}
		void EmptyLogs() {
			logs.clear();
		}
		static Logger *instance() {
			if(!static_instance)
				static_instance = new Logger;
			return static_instance;
		}
};

/**
 * Inter process communication methods between renderer and simulator
 * Utilises Protocol Buffers for serializing structured data
 */
	IPC_EXPORT void StateTransfer (std::shared_ptr<state::State> StateVar);
	IPC_EXPORT void StoreTerrain(state::Terrain TerrainVar, std::string filename);
	IPC_EXPORT state::Terrain LoadTerrain(std::string filename);
	IPC_EXPORT void IncomingInterrupts(Interrupts* InterruptVar);
}
#endif // IPC_IPC_H
