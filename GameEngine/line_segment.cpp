#include "line_segment.h"
#include "exceptions.h"

template<class T, uint n>
inline LineSegment<T, n>::LineSegment(Vector<T, n>& in_point1, Vector<T, n>& in_point2) :
	mPoint1(in_point1),
	mPoint2(in_point2) {}

template<class T, uint n>
inline void LineSegment<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	mPoint1 = in_transform.Local_To_World_Point(mPoint1);
	mPoint2 = in_transform.Local_To_World_Point(mPoint2);
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::Get_Point1() const {
	return mPoint1;
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::Get_Point2() const {
	return mPoint2;
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::Get_Center() const {
	return (mPoint1 + mPoint2) / 2.0;
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::Get_Offset() const {
	return mPoint2 - mPoint1;
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::Get_Direction() const {
	return (mPoint2 - mPoint1).Normalized();
}

template<class T, uint n>
inline T LineSegment<T, n>::Get_Projection_Coefficient1() const {
	return Get_Direction().Dot(mPoint1);
}

template<class T, uint n>
inline T LineSegment<T, n>::Get_Projection_Coefficient2() const {
	return Get_Direction().Dot(mPoint2);
}

template<class T, uint n>
inline T LineSegment<T, n>::Get_Projection_Coefficient(const Vector<T, n>& in_point) const {
	return Get_Direction().Dot(in_point);
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::Get_Projection(const Vector<T, n>& in_point) const {
	return (in_point - mPoint1).Projection(Get_Direction()) + mPoint1;
}

template<class T, uint n>
inline T LineSegment<T, n>::Get_Length() {
	return (mPoint2 - mPoint1).Magnitude();
}

template<class T, uint n>
inline Vector<T, n> LineSegment<T, n>::Random_Point() const {
	return mPoint1 + (mPoint2 - mPoint1) * Random<T>(1);
}

template<class T, uint n>
inline LineSegment<T, n> LineSegment<T, n>::From_Points(Vector<T, n>& in_point1, Vector<T, n>& in_point2) {
	return LineSegment<T, n>(in_point1, in_point2);
}

template<class T, uint n>
inline LineSegment<T, n> LineSegment<T, n>::From_Point_Offset(Vector<T, n>& in_point, Vector<T, n>& in_offset) {
	return LineSegment<T, n>(in_point, in_point + in_offset);
}

template class LineSegment<float, 2>;
template class LineSegment<double, 2>;
template class LineSegment<float, 3>;
template class LineSegment<double, 3>;