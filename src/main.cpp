#include <iostream>
#include <string>

#include <boost/spirit/include/qi.hpp>

// This include is crucial to write semantix actions in such form:
// qi::int_[qi::_val += qi::_1]
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/fusion/include/adapt_struct.hpp>


namespace qi = boost::spirit::qi;

using namespace std;


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

/**
 * Sums comma separated numbers in alternative way - using Boost.Phoenix
 * for semantic actions.
 *
 * @param str comma separated number list string.
 * @return number sum.
 */
int sum_csv_numbers_alt(const string& str);

/**
 * Runs number parsing samples.
 */
void run_basic_samples(void);

/**
 * Parses simple file matching pattern. E.g. hidden file name matching pattern
 * could be "home/user/.*". Parser tokenizes file/directory names and
 * expansions symbol '*'. Then you can easily traverse std::vector trough
 * path items 'home', 'user', '.' and check if the yalso have expansion symbol.
 *
 * @param pattern pattern string to parse.
 */
void parse_file_match_pattern(const string& pattern);


int
main(void)
{
/*
	parse_number_semantic_action("1024", [](int num) {
		cout << "Parsed: " << num << '\n';
	});
*/

	//cout << "CSV number sum: " << sum_csv_numbers("1,2,3") << '\n';
	//cout << "CSV number sum: " << sum_csv_numbers_alt("1,2,3") << '\n';

	parse_file_match_pattern("part*");
	parse_file_match_pattern("home");
	parse_file_match_pattern("home/povilas/prefix*");

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
	int result = 0;
	auto sum = [&](int num) {
		result += num;
	};
	qi::rule<string::const_iterator, int()> number_rule(
		qi::int_[sum] >> *(',' >> qi::int_[sum]));

	bool match = qi::parse(str.begin(), str.end(), number_rule);
	if (!match) {
		cout << "Int parse failed." << '\n';
	}

	return result;
}


int
sum_csv_numbers_alt(const string& str)
{
	qi::rule<string::const_iterator, int()> number_rule(
		qi::eps[qi::_val = 0] >>
		qi::int_[qi::_val += qi::_1] >> *(','
			>> qi::int_[qi::_val += qi::_1]));

	int result = -1;
	bool match = qi::parse(str.begin(), str.end(), number_rule, result);
	if (!match) {
		cout << "Int parse failed." << '\n';
	}

	return result;
}


struct file_pattern {
	string file_part;
	bool expansion;
};

BOOST_FUSION_ADAPT_STRUCT(
	file_pattern,
	(string, file_part)
	(bool, expansion)
)

void
parse_file_match_pattern(const string& pattern)
{
	qi::rule<string::const_iterator, char()> expansion(qi::char_('*'));
	qi::rule<string::const_iterator, char()> separator(qi::char_('/'));
	// All characters except special symbols '*' and '/'.
	qi::rule<string::const_iterator, string()> file_name(
		+(qi::char_ - (expansion | separator)));

	qi::rule<string::const_iterator, struct file_pattern()> path_part(
		qi::hold[file_name >> expansion]
		| (file_name >> qi::attr(false)));

	qi::rule<string::const_iterator, std::vector<struct file_pattern>()>
		path(path_part % separator);

	//struct file_pattern result{"", '\0'};
	std::vector<struct file_pattern> result;
	bool match = qi::parse(pattern.begin(), pattern.end(), path,
		result);
	if (!match) {
		cout << "Bad file pattern." << '\n';
	}

	for (auto fpatt : result) {
	cout << "File pattern: " << fpatt.file_part << " " << "; expansion: "
		<< fpatt.expansion << '\n';
	}
	cout << '\n';
}
