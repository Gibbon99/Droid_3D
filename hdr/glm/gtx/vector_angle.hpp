///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Mathematics (glm.g-truc.net)
///
/// Copyright (c) 2005 - 2013 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref gtx_vector_angle
/// @file glm/gtx/vector_angle.hpp
/// @date 2005-12-30 / 2011-06-07
/// @author Christophe Riccio
///
/// @see core (dependence)
/// @see gtx_quaternion (dependence)
/// @see gtx_epsilon (dependence)
///
/// @defgroup gtx_vector_angle GLM_GTX_vector_angle
/// @ingroup gtx
///
/// @brief Compute angle between vectors
///
/// <glm/gtx/vector_angle.hpp> need to be included to use these functionalities.
///////////////////////////////////////////////////////////////////////////////////

#ifndef GLM_GTX_vector_angle
#define GLM_GTX_vector_angle GLM_VERSION

// Dependency:
#include "../glm.hpp"
#include "../gtc/epsilon.hpp"
#include "../gtx/quaternion.hpp"
#include "../gtx/rotate_vector.hpp"

#if(defined(GLM_MESSAGES) && !defined(glm_ext))
#	pragma message("GLM: GLM_GTX_vector_angle extension included")
#endif

namespace glm
{
/// @addtogroup gtx_vector_angle
/// @{

//! Returns the absolute angle between two vectors
//! Parameters need to be normalized.
//! From GLM_GTX_vector_angle extension
template <typename vecType>
GLM_FUNC_QUALIFIER typename vecType::value_type angle(
    vecType const & x,
    vecType const & y);

//! Returns the oriented angle between two 2d vectors
//! Parameters need to be normalized.
//! From GLM_GTX_vector_angle extension.
template <typename T>
GLM_FUNC_QUALIFIER T orientedAngle(
    detail::tvec2<T> const & x,
    detail::tvec2<T> const & y);

//! Returns the oriented angle between two 3d vectors based from a reference axis.
//! Parameters need to be normalized.
//! From GLM_GTX_vector_angle extension.
template <typename T>
GLM_FUNC_QUALIFIER T orientedAngle(
    detail::tvec3<T> const & x,
    detail::tvec3<T> const & y,
    detail::tvec3<T> const & ref);

/// @}
}// namespace glm

#include "vector_angle.inl"

#endif//GLM_GTX_vector_angle
