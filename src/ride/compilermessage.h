#ifndef RIDE_COMPILERMESSAGE_H_
#define RIDE_COMPILERMESSAGE_H_

#include <ride/wx.h>

#include "ride/path.h"

class CompilerMessage
{
public:

	enum Type
	{
		TYPE_UNKNOWN,
		TYPE_NOTE,
		TYPE_WARNING,
		TYPE_ERROR,
		TYPE_RELATED
	};

	enum Source
	{
		SOURCE_RUSTC,
		SOURCE_PROTOC
	};

	CompilerMessage(
		const Fil& file,
		int start_line,
		int start_index,
		int end_line,
		int end_index,
		Type type,
		wxString message
	);
	static std::optional<CompilerMessage> Parse(const Source source, const std::optional<Dir>& root, const wxString& text );

	wxString ToStringRepresentation(const Source source);

	const Fil& file() const;

	// 1 based
	int start_line() const;

	// 1 based
	int start_index() const;

	// 1 based
	int end_line() const;

	// 1 based
	int end_index() const;
	Type type() const;
	const wxString& message() const;

private:

	Fil file_;
	int start_line_;
	int start_index_;
	int end_line_;
	int end_index_;
	Type type_;
	wxString message_;
};

#endif	// RIDE_COMPILERMESSAGE_H_
