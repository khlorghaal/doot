#pragma once

#include "global.hpp"
#include "vector.hpp"

class inbuf;
class outbuf;

struct serial{
	virtual ~serial()= default;
	virtual void serialize(outbuf&)=0;
};

class outbuf{
public:
	virtual ~outbuf()=default;

	template<typename T>
	inline outbuf& operator<<(T const& x){
		write((void*)&x, sizeof(T));
		return *this;
	}

	template<serial*>
	inline outbuf& operator<<(serial* s){
		s->serialize(*this);
		return *this;
	}

protected:
	virtual void write(const void*, size_t)=0;
};

class inbuf{
public:
	virtual ~inbuf()=default;

	template<typename T>
	inline inbuf& operator>>(T& x){
		read((void*)&x, sizeof(T));
		return *this;
	}
	template<typename T>
	inline T get(){
		T t;
		this>>t;
		return t;
	}

protected:
	virtual void read(void*, size_t)=0;
};



//DOES NOT FREE MEMORY
class bytebuf : public outbuf, public inbuf{
public:
	uint8* start;
	size_t pos, size, cap;
	//TODO document how expansion invalidates deletion
	//TODO should use sharedptrs

	bytebuf(uint8*,size_t);

	void clear();
	void flip();

protected:
	void write(const void*, size_t) override;
	void read(void*, size_t) override;
};



class vecbuf : public outbuf, public inbuf{
private:
	size_t pos=0;
public:
	vector<uint8> v;
protected:
	void write(const void*, size_t) override;
	void read(void*, size_t) override;
};



class filebuf : public outbuf, public inbuf{

};
