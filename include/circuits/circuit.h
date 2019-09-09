#ifndef __CIRCUIT_H
#define __CIRCUIT_H

#include "circuits/circuit.h"

namespace p4aig
{
	/**
	 * Circuit - A sequential circuit class to hold the circuit components
	 * and organize them for easier lookups
	 */
	class Circuit {
	private:
		Register *m_ctrlreg;

	};
}

#endif /* circuit.h */
