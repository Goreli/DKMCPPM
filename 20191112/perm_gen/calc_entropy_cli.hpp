/* calc_entropy_cli.hpp
Declares the command line parser of the Entropy Calculator application.

Copyright(c) 2019 David Krikheli

Modification history:
     1/Dec/2019 - David Krikheli created the module.
*/

#ifndef calc_entropy_cli_hpp
#define calc_entropy_cli_hpp

#include "cli_parser_base.hpp"

namespace dk {

	class ECCLIParser : public dk::CLIParserBase {
	public:
		ECCLIParser(int, char**);
		void parse();
		static void printUsage() noexcept;

		const std::string& getFilePath() const noexcept;
		bool binary() const noexcept;
		bool frequencyTable() const noexcept;
		bool numericFormat() const noexcept;
		bool help() const noexcept;
		double logBase() const noexcept;

	private:
		std::string strFilePath_;
		bool bBinary_;
		bool bFrequencyTable_;
		bool bNumericFormat_;
		bool bHelp_;
		double dblLogBase_;
	};

}	// namespace dk {
#endif // calc_entropy_cli_hpp
