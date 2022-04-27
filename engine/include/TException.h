#pragma once
#ifndef TException_H
#define TException_H
#include <exception>
#include "TObject.h"

namespace Turbo
{
	namespace Core
	{
		class TException : public std::exception, public Turbo::Core::TObject
		{
		private:
			TResult result;
			std::string message;

		public:
			TException();
			TException(TResult result, std::string message = "");
			~TException();

		public:
			TResult GetResult();
			std::string GetMessage();

			virtual std::string ToString() override;
			virtual const char *what() const throw() override;
		};
	}
}
#endif // !TException_H