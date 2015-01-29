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

#ifndef AXE_TERMINAL_H
#define AXE_TERMINAL_H

#include <array>
#include <iterator>
#include <string>
#include <math.h>
#include <utility>
#include <stddef.h>

#include "axe_result.h"
#include "axe_predicate.h"
#include "axe_composite.h"

namespace axe {

//-------------------------------------------------------------------------
/// r_bool_t rule allows including boolean expression evaluated at parse time
//-------------------------------------------------------------------------
template<class BoolT, bool = std::is_convertible<BoolT, bool>::value>
class r_bool_t {
    BoolT b_; // lambda function returning bool
  public:
    explicit r_bool_t(BoolT&& b) : b_(b) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        return make_result(b_(), i1);
    }
};
// specialization for bool convertible types
template<class BoolT>
class r_bool_t<BoolT, true> {
    BoolT b_; //
  public:
    explicit r_bool_t(BoolT&& b) : b_(b) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        return make_result(b_, i1);
    }
};

//-------------------------------------------------------------------------
/// Empty rule always return match and doesn't advance iterator
//-------------------------------------------------------------------------
struct r_empty_t {
    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        return make_result(true, i1);
    }
};

//-------------------------------------------------------------------------
/// rule to perform single character match
//-------------------------------------------------------------------------
template<class CharT>
class r_char_t {
    CharT t;

  public:
    explicit r_char_t(CharT t) : t(t) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        static_assert(std::is_convertible<decltype(*i1), CharT>::value, "*i1 must be convertible to CharT");
        return i1 != i2 ?
               make_result(t == *i1, i1 + 1, i1)
               : make_result(false, i1);
    }
};

//-------------------------------------------------------------------------
/// rule to perform token match for input sequence of tokens
//-------------------------------------------------------------------------
template<class T>
class r_token_t {
    T token_;

  public:
    explicit r_token_t(T&& token) : token_(std::forward<T>(token)) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        return i1 != i2 ?
               make_result(*i1 == token_, i1 + 1, i1)
               : make_result(false, i1);
    }
};

//-------------------------------------------------------------------------
/// rule to perform binary match
//-------------------------------------------------------------------------
template<class T>
class r_bin_t {
    T t;

  public:
    explicit r_bin_t(T&& t) : t(std::forward<T>(t)) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        static_assert(sizeof(*i1) == 1, "iterator must be byte size for binary match");

        bool matched =  i1 != i2;
        Iterator i = i1;
        const unsigned char* p = reinterpret_cast<const unsigned char*>(&t);
        size_t s = 0;
        // compare each byte
        for(; matched && s < sizeof(T) && i != i2; ++i, ++s)
            matched = p[s] == *i;
        // must match all bytes in T
        return make_result(matched && s == sizeof(T), i, i1);
    }
};

//-------------------------------------------------------------------------
/// rule to perform value match for 0 terminated strings
//-------------------------------------------------------------------------
template<class CharT>
class r_str_t {
    const CharT* str_;

  public:
    explicit r_str_t(const CharT* str) : str_(str) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        static_assert(std::is_convertible<decltype(*i1), CharT>::value, "*i1 must be convertible to CharT");

        // empty string always match
        if(!str_ || !str_[0])
            return make_result(true, i1);

        bool matched = i1 != i2;
        Iterator i = i1;
        size_t s = 0;

        for(; matched && str_[s] &&  i != i2; ++i, ++s)
            matched = str_[s] == *i;

        return make_result(matched && !str_[s], i, i1);
    }
};

//-------------------------------------------------------------------------
/// rule to perform value match for std::string lvalues
//-------------------------------------------------------------------------
template<class CharT, class TraitsT, class AllocT>
class r_str_t<std::basic_string<CharT, TraitsT, AllocT>&> {
    typedef std::basic_string<CharT, TraitsT, AllocT> StringT;
    const StringT& str_;

    r_str_t(StringT&& str); // = delete
  public:
    explicit r_str_t(const StringT& str) : str_(str) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        static_assert(std::is_convertible<decltype(*i1), CharT>::value, "*i1 must be convertible to CharT");

        // empty string always match
        if(str_.empty())
            return make_result(true, i1);

        bool matched = i1 != i2;
        Iterator i = i1;
        typename StringT::size_type s = 0, len = str_.length();

        for(; matched && s < len && i != i2; ++i, ++s)
            matched = str_[s] == *i;

        return make_result(matched && s == len, i, i1);
    }
};

//-------------------------------------------------------------------------
/// rule to perform value match for std::string rvalues
//-------------------------------------------------------------------------
template<class CharT, class TraitsT, class AllocT>
class r_str_t<std::basic_string<CharT, TraitsT, AllocT>> {
    typedef std::basic_string<CharT, TraitsT, AllocT> StringT;
    StringT str_;

  public:
    r_str_t(StringT&& str) : str_(std::move(str)) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        static_assert(std::is_convertible<decltype(*i1), CharT>::value, "*i1 must be convertible to CharT");

        // empty string always match
        if(str_.empty())
            return make_result(true, i1);

        bool matched = i1 != i2;
        Iterator i = i1;
        typename StringT::size_type s = 0, len = str_.length();

        for(; matched && s < len && i != i2; ++i, ++s)
            matched = str_[s] == *i;

        return make_result(matched && s == len, i, i1);
    }
};

//-------------------------------------------------------------------------
/// rule matches a single element satisfying predicate
//-------------------------------------------------------------------------
template<class Pred>
class r_pred_t {
    Pred pred_;
  public:
    r_pred_t(Pred&& pred) : pred_(std::forward<Pred>(pred)) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        // avoiding Visual C++ checked iterator aborts
        return i1 != i2 ? make_result(pred_(*i1), i1 + 1, i1) : make_result(false, i1);
    }
};

//-------------------------------------------------------------------------
/// rule matches a string of elements satisfying predicate
//-------------------------------------------------------------------------
template<class Pred, bool = false>
class r_predstr_t {
    Pred pred_;
  public:
    r_predstr_t(Pred&& pred) : pred_(std::forward<Pred>(pred)) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        Iterator in = i1;
        for(; in != i2 && pred_(*in); ++in);

        return make_result(in != i1, in, i1);
    }
};

//-------------------------------------------------------------------------
/// rule matches a string of specified length of elements satisfying predicate
//-------------------------------------------------------------------------
template<class Pred>
class r_predstr_t<Pred, true> {
    Pred pred_;
    size_t min_occurrence_;
    size_t max_occurrence_;
  public:
    r_predstr_t(Pred&& pred, size_t min_occurrence, size_t max_occurrence)
        : pred_(std::forward<Pred>(pred)), min_occurrence_(min_occurrence), max_occurrence_(max_occurrence) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        Iterator in = i1;
        size_t count = 0;
        for(; count < max_occurrence_ && in != i2 && pred_(*in); ++in, ++count);

        return make_result(count >= min_occurrence_, in, i1);
    }
};

//-------------------------------------------------------------------------
/// r_var_t rule matches a variable of type T (binary) and reads its value
//-------------------------------------------------------------------------
template<class T>
class r_var_t {
    T& t;

  public:
    explicit r_var_t(T& t) : t(t) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        static_assert(sizeof(*i1) == 1, "iterator must be byte size for binary match");

        unsigned char* c = reinterpret_cast<unsigned char*>(&t);
        unsigned s = 0;
        Iterator i = i1;
        for(; s < sizeof(T) && i != i2; ++i, ++s)
            c[s] = *i;

        return make_result(s == sizeof(T), i);
    }
};

//-------------------------------------------------------------------------
/// r_array_t rule matches static array of rules
//-------------------------------------------------------------------------
template<class T, size_t N>
class r_array_t {
    std::array<T, N>& buf_;

  public:
    explicit r_array_t(std::array<T, N>& a) : buf_(a) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        Iterator i = i1;
        size_t s = 0;

        for(bool matched = true; matched && s < N && i != i2; ++s) {
            r_var_t<T> tmp(buf_[s]);

            result<Iterator> result = tmp(i, i2);

            i = result.position;
            matched = result.matched;
        }

        return make_result(s == N, i);
    }
};

//-------------------------------------------------------------------------
/// r_sequence_t rule matches and reads sequence of specified length
//-------------------------------------------------------------------------
template<class C>
class r_sequence_t {
    typedef typename C::value_type T;
    C& buf_;
    const size_t min_occurrence_;
    const size_t max_occurrence_;

  public:
    //----------------------------
    r_sequence_t(C& buf, size_t min_occurrence, size_t max_occurrence)
        : buf_(buf), min_occurrence_(min_occurrence), max_occurrence_(max_occurrence) {
        buf_.clear();
    }

    //----------------------------
    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        size_t s = 0;

        for(bool matched = true; matched && i1 != i2 && s < max_occurrence_; ++s) {
            T t;
            result<Iterator> r = r_var_t<T>(t)(i1, i2);
            i1 = r.position;
            matched = r.matched;
            if(matched)
                buf_.push_back(std::move(t));
        }

        return make_result(buf_.size() >= min_occurrence_, i1);
    }
};

//-------------------------------------------------------------------------
/// r_ident_t rule matches identifier
//-------------------------------------------------------------------------
struct r_ident_t {
    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        result<Iterator> r = r_pred_t<is_alpha>(is_alpha())(i1, i2);
        if(r.matched)
            r = r_predstr_t<is_alnum>(is_alnum())(r.position, i2);
        return r;
    }
};

//-------------------------------------------------------------------------
/// r_end_t matches end of parsing range
//-------------------------------------------------------------------------
class r_end_t {
  public:
    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        return make_result(i1 == i2, i1);
    }
};

//-------------------------------------------------------------------------
/// r_advance_t succeeds when it can advance iterator by specified offset
//-------------------------------------------------------------------------
template<class OffsetT>
class r_advance_t {
    OffsetT offset_;
  public:

    r_advance_t(OffsetT&& offset) : offset_(std::forward<OffsetT>(offset)) {}

    template<class Iterator>
    result<Iterator> operator() (Iterator i1, Iterator i2) const {
        static_assert(std::is_convertible<OffsetT,
                      typename std::iterator_traits<Iterator>::difference_type>::value,
                      "offset type must be convertible to difference_type");

        bool can_advance = std::distance(i1, i2) >=
                           static_cast<typename std::iterator_traits<Iterator>::difference_type>(offset_);

        if(can_advance)
            std::advance(i1, offset_);

        return make_result(can_advance, i1);
    }
};

}

#endif
