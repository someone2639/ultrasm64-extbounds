#pragma once

/**
 * Implements the IS_ENABLED functionality using some preprocessor magic.
 *  This is a coding style used in basically every codebase with an
 *  extensive set of config options. It lets us evaluate code paths at
 *  compile time rather than preprocessing them out, with no effect on code size.
 *  (Importantly, this lets us check syntax _on all paths_ so we don't have
 *   secret config combinations that error out)
 */

// First, we define 2 cases that the preprocessor will evaluate

// `#define SOMETHING 1` will eventually reach this path
#define __ARG_PLACEHOLDER_1 0,
// `#define SOMETHING` will eventually reach this path
#define __ARG_PLACEHOLDER_ 0,

/**
 * These macros evaluate a define several times, eventually sending either a
 *  1 or a 0 to ____is_defined
 */
#define __take_second_arg(__ignored, val, ...) val
#define __is_defined(x)         ___is_defined(x)
#define ___is_defined(val)      ____is_defined(__ARG_PLACEHOLDER_##val)
#define ____is_defined(arg1_or_junk)    __take_second_arg(arg1_or_junk 1, 0)


// Finally, define IS_ENABLED in terms of the above macros
#define IS_ENABLED(def) ____is_defined(def)
