/*
 * Evaluator.h
 *
 *  Created on: 15-Feb-2017
 *      Author: Pratik Mankawde
 */

#ifndef EVALUATOR_H_
#define EVALUATOR_H_

namespace evaluators {

class Evaluator {
public:
	Evaluator();
	virtual ~Evaluator();

	virtual bool evaluate(void* aArgs) = 0;

};

} /* namespace evaluators */
#endif /* EVALUATOR_H_ */
