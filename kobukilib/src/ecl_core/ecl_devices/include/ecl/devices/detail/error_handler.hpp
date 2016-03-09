/**
 * @file /include/ecl/devices/detail/error_handler.hpp
 *
 * @brief Defines error handlers for posix devices.
 *
 * @date September 2009
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef ECL_DEVICES_ERROR_HANDLER_HPP_
#define ECL_DEVICES_ERROR_HANDLER_HPP_

/*****************************************************************************
** Cross Platform Check
*****************************************************************************/

#include <ecl/config/ecl.hpp>

/*****************************************************************************
** Includes
*****************************************************************************/

#include <string>
#include <ecl/errors/handlers.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace ecl {
namespace devices {

/*****************************************************************************
** Methods
*****************************************************************************/

/**
 * This function generates a custom ecl::Error response
 * for posix error numbers generated by posix <i>open/fdopen</i> calls.
 */
ecl::Error open_error();
/**
 * This function generates a custom ecl::Error response
 * for posix error numbers generated by posix <i>close/fclose</i> calls.
 */
ecl::Error close_error();
/**
 * This function generates a custom ecl::Error response
 * for posix error numbers generated by posix <i>fsync</i> calls.
 */
ecl::Error sync_error();
/**
 * This function generates a custom ecl::Error response
 * for posix error numbers generated by posix <i>write</i> calls.
 */
ecl::Error write_error();
/**
 * This function generates a custom ecl::Error response
 * for posix error numbers generated by posix <i>read</i> calls.
 */
ecl::Error read_error();

} // namespace devices
} // namespace ecl

#endif /* ECL_DEVICES_ERROR_HANDLER_HPP_ */