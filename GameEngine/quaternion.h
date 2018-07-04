#ifndef QUATERNION_H
#define QUATERNION_H

template<class T>
class Quaternion : public Vector<T, 4> {
public:
	using Vector::Vector;

	Quaternion<T> Hamilton(const Quaternion& q) const;
	Quaternion<T> Inverse() const;
	void Invert();
};

#endif