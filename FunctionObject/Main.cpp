#include <iostream>
#include <functional>

//boost
#define BOOST_RESULT_OF_USE_DECLTYPE
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/invoke.hpp>


template<typename... Values>
class ClosureObject
{
	boost::fusion::vector<Values...> v;

public:
	ClosureObject(Values... args)
	{
		v = boost::fusion::vector<Values...>(std::forward<Values>(args)...);
	}

	template<class... Values>
	void Set(Values... args)
	{
		v = boost::fusion::vector<Values...>(std::forward<Values>(args)...);
	}

	template<class Function>
	void Action(Function&& f)
	{
		boost::fusion::invoke(f, v);
	}
};


template<typename Function, typename... Values>
class FuncObject
{
	std::function<Function> f;
	boost::fusion::vector<Values...> v;

public:
	FuncObject(Values... args)
	{
		v = boost::fusion::vector<Values...>(std::forward<Values>(args)...);
	}

	void SetFunc(std::function<Function> pF)
	{
		f = pF;
	}

	template<class... Values>
	void Set(Values... args)
	{
		v = boost::fusion::vector<Values...>(std::forward<Values>(args)...);
	}

	void Action()
	{
		boost::fusion::invoke(f, v);
	}
};


int main()
{
	//Ç±ÇÃå`ÇæÇ∆ä÷êîÇÇ§Ç‹Ç¢Ç±Ç∆ï€éùÇ≈Ç´Ç»Ç¢
	int l = 1;
	auto co = new ClosureObject<int, int>(50, 10);
	co->Action([l](int a, int b){ std::cout << "a" << a << " / b:" << b << std::endl; });
	co->Set(500, 100);
	l = 2;
	co->Action([l](int a, int b){ std::cout << "outer var:" << l << " / a" << a << " / b:" << b << std::endl; });

	//Ç±ÇÃå`ÇæÇ∆å^êßñÒÇÇ¬ÇØÇÈë„ÇÌÇËÇ…ä÷êîÇÇ¢ÇÎÇ¢ÇÎëÄçÏÇ≈Ç´ÇÈ
	auto fo = new FuncObject<int(int, int),int,int>(50, 10);
	fo->SetFunc([](int a, int b){ std::cout << "a" << a << " / b:" << b << std::endl; return 0; });
	fo->Action();
	co->Set(500, 100);
	fo->SetFunc([l](int a, int b){ std::cout << "outer var:" << l << " / a" << a << " / b:" << b << std::endl; return 0; });
	fo->Action();
}