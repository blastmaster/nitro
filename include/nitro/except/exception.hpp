/*
 * Copyright (c) 2015-2016, Technische Universität Dresden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_NITRO_EXCEPT_EXCEPTION_HPP
#define INCLUDE_NITRO_EXCEPT_EXCEPTION_HPP

#include <sstream>
#include <stdexcept>
#include <string>

namespace nitro
{
namespace except
{

    namespace detail
    {

        template <typename Arg, typename... Args>
        class make_exception
        {
        public:
            void operator()(std::stringstream& msg, Arg&& arg, Args&&... args)
            {
                msg << std::forward<Arg>(arg);
                make_exception<Args...>()(msg, std::forward<Args>(args)...);
            }
        };

        template <typename Arg>
        class make_exception<Arg>
        {
        public:
            void operator()(std::stringstream& msg, Arg&& arg)
            {
                msg << std::forward<Arg>(arg);
            }
        };

        template <typename... Args>
        inline std::string make_string(Args&&... args)
        {
            std::stringstream msg;

            detail::make_exception<Args...>()(msg, std::forward<Args>(args)...);
            return msg.str();
        }
    } // namespace detail

    class exception : public std::runtime_error
    {
    public:
        template <typename... Args>
        explicit exception(Args&&... args)
        : std::runtime_error(detail::make_string(std::forward<Args>(args)...))
        {
        }
    };
} // namespace except
} // namespace nitro

#endif // INCLUDE_NITRO_EXCEPT_EXCEPTION_HPP
