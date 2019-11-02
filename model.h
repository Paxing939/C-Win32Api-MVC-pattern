#pragma once
#include <iterator>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <tuple>
#include <sstream>
#include <iostream>
#include <ctime>

template<typename T>
class Model;

template<typename T>
class Polynomial;

template<typename T>
class BaseIterator;

template<typename T>
class Visitor {
public:
	virtual T visit(Polynomial<T>* model) const = 0;
};

template<typename T>
class GetFirstVisitor : public Visitor<T> {
public:
	T visit(Polynomial<T>* model) const {
		if (model->IsEmpty()) {
			throw std::runtime_error("Model is empty!");
		}
		T res;
		for (int i = 0; i < model->GetDegree() + 1; i++) {
			res = model->GetCoeff(i);
			if (res != T(0)) {
				return res;
			}
		}
	}
};

template<typename T>
class GetLastVisitor : public Visitor<T> {
public:
	T visit(Polynomial<T>* model) const {
		if (model->IsEmpty()) {
			throw std::runtime_error("Model is empty!");
		}
		return model->GetCoeff(model->GetDegree());
	}
};

template<typename T>
class Model {
public:
	virtual void Add(const int& degree, const T& element) = 0;

	virtual T Get(size_t index) const = 0;

	virtual void Delete(size_t index) = 0;

	virtual BaseIterator<T> begin() = 0;

	virtual BaseIterator<T> end() = 0;

	virtual int Count() = 0;

	virtual int GetDegree() const = 0;

	virtual bool IsEmpty() const = 0;

	virtual void Clear() = 0;

	virtual T Accept(const Visitor<T>& visitor) = 0;

	std::string GetOutIter() { return str_to_out_iter_; }
	std::string GetVisFirst() { return str_for_first_; }
	std::string GetVisLast() { return str_for_last_; }

	void SetOutIter(std::string str_to_out_iter) { str_to_out_iter_ = str_to_out_iter; }
	void SetVisFirst(std::string str_for_first) { str_for_first_ = str_for_first; }
	void SetVisLast(std::string str_for_last) { str_for_last_ = str_for_last; }

	void UpdateStrings() {
		int i = this->GetDegree();
		GetFirstVisitor<T> v_first;
		GetLastVisitor<T> v_last;
		if (this->Count() > 0) {
			str_to_out_iter_ = "";
			auto iter = --this->end();
			if (*iter != T(0)) {
				str_to_out_iter_ += std::to_string(*iter) + std::string("*x^") + std::to_string(i--) + std::string(" ");
			}			
			iter--;
			for (; iter != this->begin(); iter--, i--) {
				if (*iter != 0) {
					char minusplus = '+';
					if (*iter < 0) {
						minusplus = '-';
					}
					str_to_out_iter_ += minusplus + std::string(" ") + std::to_string(fabs(*iter)) + std::string("*x^") + std::to_string(i) + std::string(" ");
				}
			}
			if (*iter != 0) {
				char minusplus = '+';
				if (*iter < 0) {
					minusplus = '-';
				}
				str_to_out_iter_ += minusplus + std::string(" ") + std::to_string(fabs(*iter));
			}
			str_for_first_ = std::to_string(this->Accept(v_first)); // output of first visitor
			str_for_last_ = std::to_string(this->Accept(v_last)); // output of last visitor
		}
		else {
			str_to_out_iter_ = "";
			str_for_first_ = "";
			str_for_last_ = "";
		}
	}

protected:
	std::string str_to_out_iter_;
	std::string str_for_first_;
	std::string str_for_last_;
};

template<typename T>
class Polynomial : public Model<T> {
public:

	void Add(const int& degree, const T& element);
	T Get(size_t index) const;
	void Delete(size_t index);

	Polynomial();
	Polynomial(int degree, const T* coefficients);
	Polynomial(const std::initializer_list<T>& elements);
	Polynomial(Polynomial<T>&& elements);
	Polynomial(const Polynomial<T>&);
	~Polynomial();

	int GetDegree() const;
	T GetCoeff(int degree) const;
	void SetDegree_(size_t degree);
	bool IsEmpty() const;
	void Insert(int degree, const T& coefficient);
	int Count();
	T* Coeff();
	void Clear();
	void Swap(Polynomial<T>& right);

	Polynomial& operator=(const Polynomial<T>&);
	bool operator==(const Polynomial<T>&) const;
	bool operator!=(const Polynomial<T>&) const;
	Polynomial<T> operator+(const Polynomial<T>&);
	Polynomial<T> operator-(const Polynomial<T>&);
	Polynomial<T> operator*(const Polynomial<T>&);
	Polynomial<T>& operator+=(const Polynomial<T>&);
	Polynomial<T>& operator-=(const Polynomial<T>&);
	Polynomial<T>& operator*=(const Polynomial<T>&);
	T& operator[](int index);

	friend std::ostream& operator<<(std::ostream&, Polynomial<T>&);
	friend std::istream& operator>>(std::istream&, Polynomial<T>&);

	BaseIterator<T> begin() {
		return BaseIterator<T>(this, 0);
	}
	BaseIterator<T> end() {
		return BaseIterator<T>(this, GetDegree() + 1);
	}

	T Accept(const Visitor<T>& visitor) {
		return visitor.visit(this);
	}

private:
	size_t degree_;
	size_t allocated_size_;
	T* coefficients_; // list of coefficients
};

template<typename T>
class BaseIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
public:
	T operator*() const {
		if (index_ > model_->GetDegree()) {
			throw std::out_of_range("Can't dereference end iterator!");
		}
		return model_->GetCoeff(index_);
	}

	T* operator->() const {
		if (index_ > model_->GetDegree()) {
			throw std::out_of_range("Can't access end iterator!");
		}
		return &model_->GetCoeff(index_);
	}

	BaseIterator& operator++() {
		if (index_ > model_->GetDegree() + 1) {
			throw std::out_of_range("Can't move further than end iterator!");
		}
		++index_;
		return *this;
	}

	const BaseIterator operator++(int) {
		BaseIterator result = *this;
		++(*this);
		return result;
	}

	BaseIterator& operator--() {
		if (index_ == 0) {
			throw std::out_of_range("Can't move earlier begin iterator!");
		}
		--index_;
		return *this;
	}

	const BaseIterator operator--(int) {
		BaseIterator result = *this;
		--(*this);
		return result;
	}

	bool operator==(const BaseIterator& other) const {
		return std::tie(model_, index_) == std::tie(other.model_, other.index_);
	}

	bool operator!=(const BaseIterator& other) const {
		return !(*this == other);
	}

private:
	friend class Polynomial<T>;

	Polynomial<T>* model_;
	size_t index_;

	BaseIterator(Polynomial<T>* model, size_t index) : model_(model), index_(index) {}
};

template <typename T>
Polynomial<T>::Polynomial() : degree_(0), allocated_size_(20) {
	coefficients_ = new T[allocated_size_];
	for (int i = 0; i < allocated_size_; i++) {
		coefficients_[i] = 0;
	}
}

template <typename T>
Polynomial<T>::Polynomial(int degree, const T* coefficients) : degree_(degree),
allocated_size_(degree * 2) {
	coefficients_ = new T[allocated_size_];
	for (int i = 0; i < allocated_size_; ++i) {
		if (i < degree_ + 1) {
			coefficients_[i] = coefficients[i];
		}
		else {
			coefficients_[i] = 0;
		}
	}
}

template <typename T>
Polynomial<T>::Polynomial(const std::initializer_list<T>& elements) : degree_(elements.size()),
allocated_size_(2 * elements.size()) {
	coefficients_ = new T[allocated_size_];

	for (int i = 0; i < allocated_size_; ++i) {
		if (i < degree_ + 1) {
			coefficients_[i] = *(elements.begin() + i);
		}
		else {
			coefficients_[i] = 0;
		}
	}
}

template <typename T>
Polynomial<T>::Polynomial(const Polynomial<T>& polynomial) : degree_(polynomial.degree_),
allocated_size_(polynomial.allocated_size_) {
	coefficients_ = new T[allocated_size_];

	for (int i = 0; i < allocated_size_; i++) {
		coefficients_[i] = polynomial.coefficients_[i];
	}
}

template <typename T>
Polynomial<T>::Polynomial(Polynomial<T>&& polynomial) : degree_(polynomial.degree_),
allocated_size_(polynomial.allocated_size_) {
	coefficients_ = new T[allocated_size_];

	for (int i = 0; i < allocated_size_; i++) {
		coefficients_[i] = polynomial.coefficients_[i];
	}
}

template <typename T>
Polynomial<T>:: ~Polynomial() {
	delete[] coefficients_;
}

template <typename T>
int Polynomial<T>::GetDegree() const { return degree_; }

template <typename T>
T Polynomial<T>::GetCoeff(int degree) const { return coefficients_[degree]; }

template <typename T>
void Polynomial<T>::SetDegree_(size_t degree) { degree_ = degree; }

template <typename T>
void Polynomial<T>::Add(const int& degree, const T& element) { Insert(degree, element); }

template <typename T>
T Polynomial<T>::Get(size_t index) const { return coefficients_[index]; }

template <typename T>
void Polynomial<T>::Delete(size_t index) {
	if (index > degree_ || index < 0) {
		throw std::out_of_range("Incorrect index!");
	}
	coefficients_[index] = T(0);
	int max = 0;
	if (index == degree_) {
		for (int i = 0; i < degree_; i++) {
			if (coefficients_[i] != T(0)) {
				max = i;
			}
		}
		degree_ = max;
	}
}

template <typename T>
bool Polynomial<T>::IsEmpty() const { return degree_ == 0 && coefficients_[0] == T(0); }

template <typename T>
void Polynomial<T>::Insert(int degree, const T& coefficient) {
	if (degree < allocated_size_ - 1) {
		for (int i = 0; i < allocated_size_; i++) {
			if (i == degree) {
				coefficients_[i] += coefficient;
				break;
			}
		}
		if (degree_ < degree) {
			degree_ = degree;
		}
	}
	else {
		T* old_coefficients = new T[degree_ + 1];
		for (int i = 0; i < degree_ + 1; i++) {
			old_coefficients[i] = coefficients_[i];
		}
		delete[] coefficients_;
		allocated_size_ = 2 * degree;
		coefficients_ = new T[allocated_size_];

		for (int i = 0; i < allocated_size_; i++) {
			if (i < degree_ + 1) {
				coefficients_[i] = old_coefficients[i];
			}
			else {
				coefficients_[i] = T(0);
			}
		}
		coefficients_[degree] = coefficient;
		degree_ = degree;
	}
}

template <typename T>
int Polynomial<T>::Count() {
	int size = 0;
	for (int i = 0; i < degree_ + 1; i++) {
		if (coefficients_[i] != T(0)) {
			size++;
		}
	}
	return size;
}

template <typename T>
T* Polynomial<T>::Coeff() { return coefficients_; }

template <typename T>
void Polynomial<T>::Clear() {
	delete[] coefficients_;
	coefficients_ = new T[allocated_size_];
	degree_ = 0;
}

template <typename T>
void Polynomial<T>::Swap(Polynomial<T>& right) {
	T* tmp_coefficients = new T[degree_ + 1];
	for (int i = 0; i < degree_ + 1; i++) {
		tmp_coefficients[i] = coefficients_[i];
	}

	delete[] coefficients_;
	coefficients_ = new T[right.allocated_size_];
	for (int i = 0; i < right.degree_ + 1; i++) {
		coefficients_[i] = right.coefficients_[i];
	}

	delete[] right.coefficients_;
	right.coefficients_ = new T[allocated_size_];
	for (int i = 0; i < degree_ + 1; i++) {
		right.coefficients_[i] = tmp_coefficients[i];
	}

	int tmp_degree = degree_, tmp_alloc_size = allocated_size_;
	degree_ = right.degree_, allocated_size_ = right.allocated_size_;
	right.degree_ = tmp_degree, right.allocated_size_ = tmp_alloc_size;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& polynomial) {
	if (&polynomial == this) {
		return *this;
	}
	delete[] coefficients_;
	allocated_size_ = polynomial.allocated_size_;
	coefficients_ = new T[allocated_size_];
	degree_ = polynomial.degree_;
	for (int i = 0; i < degree_ + 1; i++) {
		coefficients_[i] = polynomial.coefficients_[i];
	}

	return *this;
}

template <typename T>
bool Polynomial<T>::operator==(const Polynomial& polynomial) const {
	if (degree_ != polynomial.degree_) {
		return false;
	}
	for (int i = 0; i < degree_ + 1; i++) {
		if (coefficients_[i] != polynomial.coefficients[i]) {
			return false;
		}
	}
	return true;
}

template <typename T>
bool Polynomial<T>::operator!=(const Polynomial& polynomial) const {
	if (degree_ != polynomial.degree_) {
		return true;
	}
	for (int i = 0; i < degree_ + 1; i++) {
		if (coefficients_[i] != polynomial.coefficients[i]) {
			return true;
		}
	}
	return false;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator+(const Polynomial& right) {
	int max = max(degree_, right.degree_);
	Polynomial<T> poly;
	T first = 0, second = 0;
	for (int i = 0; i < max + 1; i++) {
		if (i > allocated_size_ - 1) {
			first = 0;
		}
		else {
			first = coefficients_[i];
		}
		if (i > right.allocated_size_ - 1) {
			second = 0;
		}
		else {
			second = right.coefficients_[i];
		}
		poly.Insert(i, first + second);
	}
	return poly;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator-(const Polynomial& right) {
	int max = max(degree_, right.degree_);
	Polynomial<T> poly;
	T first = 0, second = 0;
	for (int i = 0; i < max + 1; i++) {
		if (i > allocated_size_ - 1) {
			first = 0;
		}
		else {
			first = coefficients_[i];
		}
		if (i > right.allocated_size_ - 1) {
			second = 0;
		}
		else {
			second = right.coefficients_[i];
		}
		poly.Insert(i, first - second);
	}
	return poly;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial& right) {
	T* res = new T[degree_ + right.degree_ + 3];
	for (int i = 0; i < degree_ + right.degree_ + 3; i++) {
		res[i] = 0;
	}
	for (int i = 0; i < degree_ + 1; ++i) {
		for (int j = 0; j < right.degree_ + 1; ++j) {
			res[i + j] += coefficients_[i] * right.coefficients_[j];
		}
	}
	Polynomial<T> poly;
	poly.degree_ = degree_ + right.degree_;
	poly.allocated_size_ = 2 * poly.degree_;
	for (int i = 0; i < poly.degree_ + 1; i++) {
		poly.Insert(i, res[i]);
	}
	return poly;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator+=(const Polynomial& right) {
	*this = *this + right;
	return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator-=(const Polynomial& right) {
	*this = *this - right;
	return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial& right) {
	*this = *this * right;
	return *this;
}

template <typename T>
T& Polynomial<T>::operator[] (int index) {
	if (index < 0 || index > degree_) {
		throw std::out_of_range("Incorrect index!");
	}
	return coefficients_[index];
}

template<typename T>
std::ostream& operator<<(std::ostream& out, Polynomial<T>& right) {
	if (right.IsEmpty()) {
		throw std::out_of_range("Trying to output empty Polynomial!");
	}
	for (int i = right.GetDegree(); i >= 0; i--) {
		if (right[i] != 0) {
			if (i != right.GetDegree()) {
				if (right[i] > 0) {
					out << " + ";
				}
				else {
					out << " - ";
				}
			}
			else {
				if (right[i] < 0) {
					out << "-";
				}
			}
			if (i != 0) {
				out << fabs(right[i]) << "*X^(" << i << ")";
			}
			else {
				out << fabs(right[i]);
			}
		}
	}
	return out;
}

// input form should be: <element> <degree>
template<typename T>
std::istream& operator>>(std::istream& in, Polynomial<T>& right) {
	std::string line;
	int degree;
	while (getline(in, line)) {
		std::stringstream line_reader(line);
		T element;
		while (line_reader >> element) {
			line_reader >> degree;
			right.Insert(degree, element);
		}
	}

	return in;
}

/*emplate <typename T>
Polynomial<T>& GenerateRandPoly() {
	srand(time(NULL));
	int degree = 3;
	int allocated_size = 2 * degree;
	T coefficients = new T[allocated_size];
	for (int i = 0; i < degree + 1; i++) {
		coefficients[i] = rand % 10 - 5;
	}
	return Polynomial<T>(degree, coefficients);
}*/
