//-----------------------------------------------------------------------------
//  Original Author: Gene Bushuyev
//  Copyright (C) 2011 GB Research, LLC
//
//  Boost Software License - Version 1.0 - August 17th, 2003
//
//  Permission is hereby granted, free of charge, to any person or organization
//  obtaining a copy of the software and accompanying documentation covered by
//  this license (the "Software") to use, reproduce, display, distribute,
//  execute, and transmit the Software, and to prepare derivative works of the
//  Software, and to permit third-parties to whom the Software is furnished to
//  do so, all subject to the following:
//
//  The copyright notices in the Software and this entire statement, including
//  the above license grant, this restriction and the following disclaimer,
//  must be included in all copies of the Software, in whole or in part, and
//  all derivative works of the Software, unless such copies or derivative
//  works are solely in the form of machine-executable object code generated by
//  a source language processor.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//  FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef AXE_COMPOSITE_FUNCTION_H
#define AXE_COMPOSITE_FUNCTION_H

#include <type_traits>
#include <utility>
#include <functional>
#include "axe_trait.h"
#include "axe_composite.h"
#include "axe_terminal.h"

namespace axe {

//-------------------------------------------------------------------------
// operators for composite rules
//-------------------------------------------------------------------------

/// binary operator& matches rule r1 followed by rule r2
template<class R1, class R2>
r_and_t<
typename std::enable_if<is_rule<typename std::remove_reference<R1>::type>::value, R1>::type,
         typename std::enable_if<is_rule<typename std::remove_reference<R2>::type>::value, R2>::type
         >
operator& (R1&& r1, R2&& r2) {
    return r_and_t<R1, R2>(std::forward<R1>(r1), std::forward<R2>(r2));
}

//-------------------------------------------------------------------------
/// binary operator| matches rule r1 or r2
template<class R1, class R2>
r_or_t<
typename std::enable_if<is_rule<typename std::remove_reference<R1>::type>::value, R1>::type,
         typename std::enable_if<is_rule<typename std::remove_reference<R2>::type>::value, R2>::type
         >
operator| (R1&& r1, R2&& r2) {
    return r_or_t<R1, R2>(std::forward<R1>(r1), std::forward<R2>(r2));
}

//-------------------------------------------------------------------------
/// operator| called with second r_fail rule, calls specified function on failure
template<class R1, class R2>
r_fail_t<
typename std::enable_if<is_rule<typename std::remove_reference<R1>::type>::value, R1>::type,
         R2
         >
operator| (R1&& r1, const r_fail_wrapper_t<R2>& r2) {
    return r_fail_t<R1, R2>(std::forward<R1>(r1), r2.get());
}

//-------------------------------------------------------------------------
/// operator^ matches either the first or the second rule, but not both (r1 & !r2 | !r1 & r2)
template<class R1, class R2>
r_xor_t<
typename std::enable_if<is_rule<typename std::remove_reference<R1>::type>::value, R1>::type,
         typename std::enable_if<is_rule<typename std::remove_reference<R2>::type>::value, R2>::type
         >
operator^ (R1&& r1, R2&& r2) {
    return r_xor_t<R1, R2>(std::forward<R1>(r1), std::forward<R2>(r2));
}

//-------------------------------------------------------------------------
/// operator! succeeds on mismatch of the specified rule
template<class R>
r_not_t<
typename std::enable_if<is_rule<typename std::remove_reference<R>::type>::value, R>::type
>
operator! (R&& r) {
    return r_not_t<R>(std::forward<R>(r));
}

//-------------------------------------------------------------------------
/// operator~ specifies an optional rule (matched 0 or 1 times)
template<class R>
r_opt_t<
typename std::enable_if<is_rule<typename std::remove_reference<R>::type>::value, R>::type
>
operator~ (R&& r) {
    return r_opt_t<R>(std::forward<R>(r));
}

//-------------------------------------------------------------------------
// convenience operators
//-------------------------------------------------------------------------

/// unary operator* matches specified rule 0 or more times
template<class R>
r_many_t<
typename std::enable_if<is_rule<typename std::remove_reference<R>::type>::value, R>::type,
         r_empty_t
         >
operator* (R&& r) {
    return r_many_t<R, r_empty_t>(std::forward<R>(r), r_empty_t(), 0, -1);
}

/// unary operator+ matches specified rule 1 or more times
template<class R>
r_many_t<
typename std::enable_if<is_rule<typename std::remove_reference<R>::type>::value, R>::type,
         r_empty_t
         >
operator+ (R&& r) {
    return r_many_t<R, r_empty_t>(std::forward<R>(r), r_empty_t(), 1, -1);
}

/// operator% matches rule, followed by separator 1 or more times
template<class R1, class R2>
auto operator% (R1&& r1, R2&& r2) -> decltype(r_many(std::forward<R1>(r1), std::forward<R2>(r2), 1, -1)) {
    return r_many(std::forward<R1>(r1), std::forward<R2>(r2), 1, -1);
}

/// binary operator- matches first rule, but not the second (r1 - r2 is the same as !r2 & r1)
template<class R1, class R2>
r_and_t<
typename std::enable_if<is_rule<typename std::remove_reference<R1>::type>::value, r_not_t<R2>>::type,
         typename std::enable_if<is_rule<typename std::remove_reference<R2>::type>::value, R1>::type
         >
operator- (R1&& r1, R2&& r2) {
    return r_and_t<r_not_t<R2>, R1>(r_not_t<R2>(std::forward<R2>(r2)), std::forward<R1>(r1));
}

//-------------------------------------------------------------------------
// functions for composite rules
//-------------------------------------------------------------------------

/// function r_many matches specified rule, separated by another rule specified number of occurrences
template<class R, class S>
r_many_t<
typename std::enable_if<is_rule<typename std::remove_reference<R>::type>::value, R>::type,
         typename std::enable_if<is_rule<typename std::remove_reference<S>::type>::value, S>::type
         >
r_many (R&& r, S&& s, size_t min_occurrence = 1, size_t max_occurrence = -1) {
    return r_many_t<R, S>(std::forward<R>(r), std::forward<S>(s), min_occurrence, max_occurrence);
}

//-------------------------------------------------------------------------
template<class R>
r_many_t<
typename std::enable_if<is_rule<typename std::remove_reference<R>::type>::value, R>::type,
         r_empty_t
         >
r_many (R&& r, size_t min_occurrence = 1, size_t max_occurrence = -1) {
    return r_many_t<R, r_empty_t>(std::forward<R>(r), r_empty_t(), min_occurrence, max_occurrence);
}

//-------------------------------------------------------------------------
/// function r_ref creates a reference wrapper rule
template<class R>
r_ref_t<R>
r_ref (R&& r) {
    return r_ref_t<R>(std::forward<R>(r));
}

//-------------------------------------------------------------------------
/// function r_find skips input elements until specified rule is matched
template<class R>
r_find_t<
typename std::enable_if<is_rule<typename std::remove_reference<R>::type>::value, R>::type
>
r_find (R&& r) {
    return r_find_t<R>(std::forward<R>(r));
}

//-------------------------------------------------------------------------
/// function r_fail wraps failure function, which is executed on rule failure (e.g. rule | r_fail(fail_func))
template<class R>
r_fail_wrapper_t<R>
r_fail (R&& r) {
    return r_fail_wrapper_t<R>(std::forward<R>(r));
}

//-------------------------------------------------------------------------
/// function r_select selects rule to match based on result of first rule matching
/// r_select(r1, r2, r3) is equivalent to !r1 & r3 | r1 & r2, except r1 is matched only once
template<class R1, class R2, class R3>
r_select_t<
typename std::enable_if<is_rule<typename std::remove_reference<R1>::type>::value, R1>::type,
         typename std::enable_if<is_rule<typename std::remove_reference<R2>::type>::value, R2>::type,
         typename std::enable_if<is_rule<typename std::remove_reference<R3>::type>::value, R3>::type
         >
r_select (R1&& r1, R2&& r2, R3&& r3) {
    return r_select_t<R1, R2, R3>(std::forward<R1>(r1), std::forward<R2>(r2), std::forward<R3>(r3));
}

//-----------------------------------------------------------------------------
/// r_test matches the specified rule, but always returns the initial iterator
/// thus it allows to test the rule and perform semantic actions
//-----------------------------------------------------------------------------
template<class R>
r_test_t<R>
r_test (R&& r) {
    return r_test_t<R>(std::forward<R>(r));
}

} // namespace axe

#endif
