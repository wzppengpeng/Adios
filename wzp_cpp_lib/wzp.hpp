#ifndef WZP_CPP_LIB_H_PP_
#define WZP_CPP_LIB_H_PP_

/**
 * This is the often used header files
 */
#include "wzp_cpp_lib/uncopyable.hpp"

// the config
#include "wzp_cpp_lib/config/config_parser.hpp"
#include "wzp_cpp_lib/config/click.hpp"

// container
#include "wzp_cpp_lib/container/array_args.hpp"
#include "wzp_cpp_lib/container/array_view.hpp"
#include "wzp_cpp_lib/container/any.hpp"
#include "wzp_cpp_lib/container/variant.hpp"
#include "wzp_cpp_lib/container/optional.hpp"

// for files and directories
#include "wzp_cpp_lib/files/path.hpp"
#include "wzp_cpp_lib/files/file_loader.hpp"

// help functions
#include "wzp_cpp_lib/function/help_function.hpp"
#include "wzp_cpp_lib/function/apply_tuple.hpp"

// logging
#include "wzp_cpp_lib/log/log.hpp"

// string handle
#include "wzp_cpp_lib/my_string/char_args.hpp"
#include "wzp_cpp_lib/my_string/string.hpp"

// multi thread parallel
#include "wzp_cpp_lib/thread/parallel_algorithm.hpp"

// some util samll help functions
#include "wzp_cpp_lib/util/lexical_cast.hpp"
#include "wzp_cpp_lib/util/random.hpp"
#include "wzp_cpp_lib/util/timer.hpp"

// reflection of factory
#include "wzp_cpp_lib/reflection/factory.hpp"

#endif /*WZP_CPP_LIB_H_PP_*/