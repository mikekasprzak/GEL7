/*
* Copyright 2016 Google Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef MATHFU_INTERNAL_VECTOR_3_H_
#define MATHFU_INTERNAL_VECTOR_3_H_

#include <mathfu/vector.h>

namespace mathfu {

template <class T>
class Vector<T, 3> {
 public:
  typedef T Scalar;
  static const int Dims = 3;

  inline Vector() {}

  inline Vector(const Vector<T, 3>& v)
      : x(v.x), y(v.y), z(v.z) {}

  explicit inline Vector(const VectorPacked<T, 3>& v)
      : x(v.x), y(v.y), z(v.z) {}

  explicit inline Vector(const T* a)
      : x(a[0]), y(a[1]), z(a[2]) {}

  explicit inline Vector(T s)
      : x(s), y(s), z(s) {}

  inline Vector(T s1, T s2, T s3)
      : x(s1), y(s2), z(s3) {}

  inline Vector(const Vector<T, 2>& v12, T s3)
      : x(v12.x), y(v12.y), z(s3) {}

  template <typename U>
  explicit inline Vector(const Vector<U, 3>& v)
      : x(static_cast<T>(v.x)),
        y(static_cast<T>(v.y)),
        z(static_cast<T>(v.z)) {}

  inline T& operator()(const int i) { return data_[i]; }

  inline const T& operator()(const int i) const { return data_[i]; }

  inline T& operator[](const int i) { return data_[i]; }

  inline const T& operator[](const int i) const { return data_[i]; }

  inline Vector<T, 3> xyz() { return Vector<T, 3>(x, y, z); }

  inline const Vector<T, 3> xyz() const { return Vector<T, 3>(x, y, z); }

  inline Vector<T, 2> xy() { return Vector<T, 2>(x, y); }

  inline const Vector<T, 2> xy() const { return Vector<T, 2>(x, y); }

  inline void Pack(VectorPacked<T, 3>* const vector) const {
    vector->x = x;
    vector->y = y;
    vector->z = z;
  }

  inline T LengthSquared() const { return LengthSquaredHelper(*this); }

  inline T Length() const { return LengthHelper(*this); }

  inline T Normalize() { return NormalizeHelper(*this); }

  inline Vector<T, 3> Normalized() const { return NormalizedHelper(*this); }

  template <typename CompatibleT>
  static inline Vector<T, 3> FromType(const CompatibleT& compatible) {
    return FromTypeHelper<T, Dims, CompatibleT>(compatible);
  }

  template <typename CompatibleT>
  static inline CompatibleT ToType(const Vector<T, 3>& v) {
    return ToTypeHelper<T, Dims, CompatibleT>(v);
  }

  static inline T DotProduct(const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
    return DotProductHelper(v1, v2);
  }

  static inline Vector<T, 3> HadamardProduct(const Vector<T, 3>& v1,
                                             const Vector<T, 3>& v2) {
    return HadamardProductHelper(v1, v2);
  }

  static inline Vector<T, 3> CrossProduct(const Vector<T, 3>& v1,
                                          const Vector<T, 3>& v2) {
    return CrossProductHelper(v1, v2);
  }

  static inline Vector<T, 3> Lerp(const Vector<T, 3>& v1,
                                  const Vector<T, 3>& v2, const T percent) {
    return LerpHelper(v1, v2, percent);
  }

  static inline Vector<T, 3> RandomInRange(const Vector<T, 3>& min,
                                           const Vector<T, 3>& max) {
    return RandomInRangeHelper(min, max);
  }

  static inline Vector<T, 3> Max(const Vector<T, 3>& v1,
                                 const Vector<T, 3>& v2) {
    return MaxHelper(v1, v2);
  }

  static inline Vector<T, 3> Min(const Vector<T, 3>& v1,
                                 const Vector<T, 3>& v2) {
    return MinHelper(v1, v2);
  }

  static inline T Distance(const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
    return (v1 - v2).Length();
  }

  static inline T DistanceSquared(const Vector<T, 3>& v1,
                                  const Vector<T, 3>& v2) {
    return (v1 - v2).LengthSquared();
  }

  static inline T Angle(const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
    return AngleHelper(v1, v2);
  }

  MATHFU_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE

#include "mathfu/internal/disable_warnings_begin.h"
  union {
    T data_[3];
    struct {
      T x;
      T y;
      T z;
    };
  };
#include "mathfu/internal/disable_warnings_end.h"
};

template <class T>
struct VectorPacked<T, 3> {
  /// Create an uninitialized VectorPacked.
  VectorPacked() {}

  /// Create a VectorPacked from a Vector.
  ///
  /// Both VectorPacked and Vector must have the same number of dimensions.
  /// @param vector Vector to create the VectorPacked from.
  explicit VectorPacked(const Vector<T, 3>& vector) { vector.Pack(this); }

  /// Copy a Vector to a VectorPacked.
  ///
  /// Both VectorPacked and Vector must have the same number of dimensions.
  /// @param vector Vector to copy to the VectorPacked.
  /// @returns A reference to this VectorPacked.
  VectorPacked& operator=(const Vector<T, 3>& vector) {
    vector.Pack(this);
    return *this;
  }

#include "mathfu/internal/disable_warnings_begin.h"
  /// Elements of the packed vector one per dimension.
  union {
    T data_[3];
    struct {
      T x;
      T y;
      T z;
    };
  };
#include "mathfu/internal/disable_warnings_end.h"
};

}  //  namespace mathfu

#endif  // MATHFU_INTERNAL_VECTOR_3_H_
