#include <iostream>
#include <string>

#include <boost/spirit/include/qi.hpp>


using namespace std;

namespace qi = boost::spirit::qi;

/**
 * Very simple example of number parsing.
 *
 * @param str ascii string representing integer number.
 * @return parsed number or -1 on failure.
 */
int parse_number(const string& str);

/**
 * Parses string for number and performs an action (callback, lambda, functor)
 * when a number is found.
 *
 * @param str string to parse.
 * @param act parser action called when number is found.
 */
void parse_number_semantic_action(const string& str,
	std::function<void(int)> act);

/**
 * Returns the sum of comma separated numbers. E.g. "1,2,3" would return 6.
 *
 * @param str comma separated number list string.
 * @return number sum.
 */
int sum_csv_numbers(const string& str);

void run_basic_samples(void);


int
main(void)
{
/*
	parse_number_semantic_action("1024", [](int num) {
		cout << "Parsed: " << num << '\n';
	});
*/

	cout << "CSV number sum: " << sum_csv_numbers("1, 2, 3") << '\n';

	return 0;
}


void
run_basic_samples(void)
{
	cout << "Parsed int: " << parse_number("123") << '\n';
	cout << "Parsed negative int: " << parse_number("-123") << '\n';
	cout << "Parse int failure: " << parse_number("abc-123") << '\n';

}


void
parse_number_semantic_action(const string& str, std::function<void(int)> act)
{
	qi::rule<string::const_iterator, int()> number_rule;
	number_rule = qi::int_[act];

	int result = -1;
	bool match = qi::parse(str.begin(), str.end(), number_rule, result);
	if (!match) {
		cout << "Int parse failed." << '\n';
	}
}


int
parse_number(const string& str)
{
	qi::rule<string::const_iterator, int()> number_rule;
	number_rule = qi::int_;

	int result = -1;
	bool match = qi::parse(str.begin(), str.end(), number_rule, result);
	if (!match) {
		cout << "Int parse failed." << '\n';
	}

	return result;
}


int
sum_csv_numbers(const string& str)
{
	qi::rule<string::const_iterator, int()> number_rule;
	int result = 0;
	// TODO: finish summation: int_[ref(result) += _1] or smth like that.
	number_rule = qi::int_ >> *(',' >> qi::int_);

	bool match = qi::parse(str.begin(), str.end(), number_rule);
	if (!match) {
		cout << "Int parse failed." << '\n';
	}

	return result;
}
