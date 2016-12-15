#ifndef _EXECUTOR_HPP_
#define _EXECUTOR_HPP_

namespace wzp {
    class Executor {
    public:
        virtual ~Executor() {}

        virtual void run() = 0;
    };

}//wzp

#endif /*_EXECUTOR_HPP_*/
