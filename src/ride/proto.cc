#include "ride/proto.h"

#include <fstream>
#include <set>

#include "ride/settings.proto.h"

#include "jsonh/jsonh.h"

// ------------------------------------------------------------------------------------------------
// filer
struct Filer
{
	bool is_loading;
	jsonh::Document* doc;
	jsonh::Value json;

	std::string require_string_prop(const std::string& name)
	{
		return "";
	}
};

// ------------------------------------------------------------------------------------------------
// filing

std::pair<std::string, jsonh::Location> describe(jsonh::Value val, jsonh::Document* doc)
{
	switch (val.type)
	{
	case jsonh::ValueType::Object: return {"Object", val.AsObject(doc)->location};
	case jsonh::ValueType::Array: return {"Array", val.AsArray(doc)->location};
	case jsonh::ValueType::String: return {"String", val.AsString(doc)->location};
	case jsonh::ValueType::Number: return {"Number", val.AsNumber(doc)->location};
	case jsonh::ValueType::Int: return {"Int", val.AsInt(doc)->location};
	case jsonh::ValueType::Bool: return {"Bool", val.AsBool(doc)->location};
	case jsonh::ValueType::Null: return {"Null", val.AsNull(doc)->location};
	default: assert(false);
	}
	return {"Invalid", jsonh::Location{0, 0}};
}

void add_expected(SerLog* log, const std::string& what, jsonh::Value value, jsonh::Document* doc)
{
	const auto [type, loc] = describe(value, doc);
	log->errors.emplace_back(SerError{"Expected " + what + " but found " + type, loc.line, loc.column});
}

void add_expected(SerLog* log, const std::string& what, Filer* filer)
{
	add_expected(log, what, filer->json, filer->doc);
}


void ser(SerLog* log, Filer* filer, bool* value)
{
	if(filer->is_loading)
	{
		jsonh::Bool* read = filer->json.AsBool(filer->doc);
		if (read == nullptr)
		{
			add_expected(log, "bool", filer);
			return;
		}
		*value = read->value;
	}
	else
	{
		filer->json = filer->doc->add(jsonh::Bool{{}, *value});
	}
}

void ser(SerLog* log, Filer* filer, int* value)
{
	if (filer->is_loading)
	{
		jsonh::Int* read = filer->json.AsInt(filer->doc);
		if (read == nullptr)
		{
			add_expected(log, "int", filer);
			return;
		}
		*value = read->value;
	}
	else
	{
		filer->json = filer->doc->add(jsonh::Int{{}, *value});
	}
}

void ser(SerLog* log, Filer* filer, std::string* value)
{
	if (filer->is_loading)
	{
		jsonh::String* read = filer->json.AsString(filer->doc);
		if (read == nullptr)
		{
			add_expected(log, "string", filer);
			return;
		}
		*value = read->value;
	}
	else
	{
		filer->json = filer->doc->add(jsonh::String{{}, *value});
	}
}

void ser(SerLog* log, Filer* filer, wxString* value)
{
	if (filer->is_loading)
	{
		jsonh::String* read = filer->json.AsString(filer->doc);
		if (read == nullptr)
		{
			add_expected(log, "string", filer);
			return;
		}
		*value = read->value;
	}
	else
	{
		filer->json = filer->doc->add(jsonh::String{{}, value->utf8_string()});
	}
}



template<typename T>
struct EnumBuilder
{
	std::map<T, std::string> enum_to_name;
	std::map<std::string, T> name_to_enum;

	void add(T t, const std::string& n)
	{
		enum_to_name[t] = n;
		name_to_enum[n] = t;
	}

	void complete(const std::string& name, SerLog* log, Filer* filer, T* value)
	{
		if (filer->is_loading)
		{
			jsonh::String* string = filer->json.AsString(filer->doc);
			if (string == nullptr)
			{
				add_expected(log, name + "as a string", filer);
				return;
			}
			const auto found = name_to_enum.find(string->value);
			if (found == name_to_enum.end())
			{
				log->errors.emplace_back(
					SerError{
						name + " enum not a valid string",
						string->location.line,
						string->location.column
					}
				);
				return;
			}
			*value = found->second;
		}
		else
		{
			const auto found = enum_to_name.find(*value);
			std::string val = "???";
			if (found != enum_to_name.end())
			{
				val = found->second;
			}
			else
			{
				assert(false && "enum value not found in enum builder");
			}

			filer->json = filer->doc->add(jsonh::String{{}, val});
		}
	}
};

#define F_ENUM(ENUM) void ser(SerLog* log, Filer* filer, ride::ENUM* value)
#define S_ENUM_DECLARE(ENUM) const std::string_view name = #ENUM; EnumBuilder<ride::ENUM> builder;
#define S_ENUM_VAL(x, n) builder.add(ride::x, n)
#define S_ENUM_COMPLETE() builder.complete(std::string(name), log, filer, value)

F_ENUM(EdgeStyle)
{
	S_ENUM_DECLARE(EdgeStyle);
	S_ENUM_VAL(EDGESTYLE_NONE, "none");
	S_ENUM_VAL(EDGESTYLE_LINE, "line");
	S_ENUM_VAL(EDGESTYLE_BACKGROUND, "background");
	S_ENUM_COMPLETE();
}

F_ENUM(ViewWhitespace)
{
	S_ENUM_DECLARE(ViewWhitespace);
	S_ENUM_VAL(VIEWWHITESPACE_HIDDEN, "hidden");
	S_ENUM_VAL(VIEWWHITESPACE_ALWAYS, "always");
	S_ENUM_VAL(VIEWWHITESPACE_AFTER_IDENT, "after_ident");
	S_ENUM_COMPLETE();
}

F_ENUM(WrapMode)
{
	S_ENUM_DECLARE(WrapMode);
	S_ENUM_VAL(WRAPMODE_NONE, "none");
	S_ENUM_VAL(WRAPMODE_CHAR, "char");
	S_ENUM_VAL(WRAPMODE_WORD, "word");
 S_ENUM_COMPLETE();
}

F_ENUM(AutoIndentation)
{
	S_ENUM_DECLARE(AutoIndentation);
	S_ENUM_VAL(AUTOINDENTATION_NONE, "none");
	S_ENUM_VAL(AUTOINDENTATION_KEEP, "keep");
	S_ENUM_VAL(AUTOINDENTATION_SMART, "mart");
	S_ENUM_COMPLETE();
}

F_ENUM(IndicatorStyle)
{
	S_ENUM_DECLARE(IndicatorStyle);
	S_ENUM_VAL(INDICATORSTYLE_PLAIN, "plain");
	S_ENUM_VAL(INDICATORSTYLE_SQUIGGLE, "squiggle");
	S_ENUM_VAL(INDICATORSTYLE_TT, "tt");
	S_ENUM_VAL(INDICATORSTYLE_DIAGONAL, "diagonal");
	S_ENUM_VAL(INDICATORSTYLE_STRIKE, "strike");
	S_ENUM_VAL(INDICATORSTYLE_HIDDEN, "hidden");
	S_ENUM_VAL(INDICATORSTYLE_BOX, "box");
	S_ENUM_VAL(INDICATORSTYLE_ROUNDBOX, "roundbox");
	S_ENUM_VAL(INDICATORSTYLE_STRAIGHTBOX, "straightbox");
	S_ENUM_VAL(INDICATORSTYLE_DASH, "dash");
	S_ENUM_VAL(INDICATORSTYLE_DOTS, "dots");
	S_ENUM_VAL(INDICATORSTYLE_SQUIGGLELOW, "squigglelow");
	S_ENUM_VAL(INDICATORSTYLE_DOTBOX, "dotbox");
	S_ENUM_COMPLETE();
};

F_ENUM(VirtualSpace)
{
	S_ENUM_DECLARE(VirtualSpace);
	S_ENUM_VAL(VIRTUALSPACE_NONE, "none");
	S_ENUM_VAL(VIRTUALSPACE_RECTANGULARSELECTION, "rectangularselection");
	S_ENUM_VAL(VIRTUALSPACE_USERACCESSIBLE, "useraccessible");
	S_ENUM_COMPLETE();
};

F_ENUM(WrapVisualFlagsLocation)
{
	S_ENUM_DECLARE(WrapVisualFlagsLocation);
	S_ENUM_VAL(WRAPVISUALFLAGLOC_DEFAULT, "default");
	S_ENUM_VAL(WRAPVISUALFLAGLOC_END_BY_TEXT, "end_by_text");
	S_ENUM_VAL(WRAPVISUALFLAGLOC_START_BY_TEXT, "start_by_text");
	S_ENUM_COMPLETE();
};

F_ENUM(WrapIndentMode)
{
	S_ENUM_DECLARE(WrapIndentMode);
	S_ENUM_VAL(WRAPINDENT_FIXED, "fixed");
	S_ENUM_VAL(WRAPINDENT_SAME, "same");
	S_ENUM_VAL(WRAPINDENT_INDENT, "indent");
	S_ENUM_COMPLETE();
};

F_ENUM(Annotation)
{
	S_ENUM_DECLARE(Annotation);
	S_ENUM_VAL(ANNOTATION_HIDDEN, "hidden");
	S_ENUM_VAL(ANNOTATION_STANDARD, "standard");
	S_ENUM_VAL(ANNOTATION_BOXED, "boxed");
	S_ENUM_COMPLETE();
};

F_ENUM(MarkerSymbol)
{
	S_ENUM_DECLARE(MarkerSymbol);
	S_ENUM_VAL(MARKERSYMBOL_CIRCLE, "circle");
	S_ENUM_VAL(MARKERSYMBOL_ROUNDRECT, "roundrect");
	S_ENUM_VAL(MARKERSYMBOL_ARROW, "arrow");
	S_ENUM_VAL(MARKERSYMBOL_SMALLRECT, "smallrect");
	S_ENUM_VAL(MARKERSYMBOL_SHORTARROW, "shortarrow");
	S_ENUM_VAL(MARKERSYMBOL_EMPTY, "empty");
	S_ENUM_VAL(MARKERSYMBOL_ARROWDOWN, "arrowdown");
	S_ENUM_VAL(MARKERSYMBOL_MINUS, "minus");
	S_ENUM_VAL(MARKERSYMBOL_PLUS, "plus");
	S_ENUM_COMPLETE();
};

F_ENUM(CaretStyle)
{
	S_ENUM_DECLARE(CaretStyle);
	S_ENUM_VAL(CARETSTYLE_INVISIBLE, "invisible");
	S_ENUM_VAL(CARETSTYLE_LINE, "line");
	S_ENUM_VAL(CARETSTYLE_BLOCK, "block");
	S_ENUM_COMPLETE();
};

F_ENUM(CaretSticky)
{
	S_ENUM_DECLARE(CaretSticky);
	S_ENUM_VAL(CARETSTICKY_OFF, "off");
	S_ENUM_VAL(CARETSTICKY_WHITESPACE, "whitespace");
	S_ENUM_VAL(CARETSTICKY_ON, "on");
	S_ENUM_COMPLETE();
};

F_ENUM(StatusbarStyle)
{
	S_ENUM_DECLARE(StatusbarStyle);
	S_ENUM_VAL(STATUSBAR_STYLE_NONE, "none");
	S_ENUM_VAL(STATUSBAR_STYLE_BAR, "bar");
	S_ENUM_VAL(STATUSBAR_STYLE_RAISED, "raised");
	S_ENUM_VAL(STATUSBAR_STYLE_SUNKEN, "sunken");
	S_ENUM_COMPLETE();
};

F_ENUM(AutoComplete)
{
	S_ENUM_DECLARE(AutoComplete);
	S_ENUM_VAL(AUTOCOMPLETE_NONE, "none");
	S_ENUM_VAL(AUTOCOMPLETE_PARA, "para");
	S_ENUM_COMPLETE();
};

F_ENUM(HomeEndStyle)
{
	S_ENUM_DECLARE(HomeEndStyle);
	S_ENUM_VAL(HES_BASIC, "basic");
	S_ENUM_VAL(HES_DISPLAY, "display");
	S_ENUM_VAL(HES_WRAP, "wrap");
	S_ENUM_VAL(HES_VCBASIC, "vcbasic");
	S_ENUM_VAL(HES_VCDISPLAY, "vcdisplay");
	S_ENUM_VAL(HES_VCWRAP, "vcwrap");
	S_ENUM_VAL(HES_SCROLL, "scroll");
	S_ENUM_COMPLETE();
};

F_ENUM(WindowState)
{
	S_ENUM_DECLARE(WindowState);
	S_ENUM_VAL(WINDOWSTATE_NORMAL, "normal");
 S_ENUM_VAL(WINDOWSTATE_ICONIZED, "iconized");
	S_ENUM_VAL(WINDOWSTATE_MAXIMIZED, "maximized");
	S_ENUM_VAL(WINDOWSTATE_FULLSCREEN, "fullscreen");
	S_ENUM_COMPLETE();
};

F_ENUM(FindDlgTarget)
{
	S_ENUM_DECLARE(FindDlgTarget);
	S_ENUM_VAL(FDT_NORMAL_TEXT, "text");
	S_ENUM_VAL(FDT_NORMAL_REGEX, "regex");
	S_ENUM_VAL(FDT_NORMAL_POSIX, "posix");
	S_ENUM_COMPLETE();
};

jsonh::Location location_of(const jsonh::Value& val, jsonh::Document* doc, const jsonh::Location& def)
{
	switch (val.type)
	{
	case jsonh::ValueType::Invalid: return def;
	case jsonh::ValueType::Object: return val.AsObject(doc)->location;
	case jsonh::ValueType::Array: return val.AsArray(doc)->location;
	case jsonh::ValueType::String: return val.AsString(doc)->location;
	case jsonh::ValueType::Number: return val.AsNumber(doc)->location;
	case jsonh::ValueType::Int: return val.AsInt(doc)->location;
	case jsonh::ValueType::Bool: return val.AsBool(doc)->location;
	case jsonh::ValueType::Null: return val.AsNull(doc)->location;
	default: return def;
	}
}

struct StructParser {
	SerLog* log;
	Filer* filer;
	std::string struct_name;
	std::set<std::string> names;
	
	jsonh::Object* get()
	{
		jsonh::Object* ret = filer->json.AsObject(filer->doc);
		assert(ret && "non object was not expected");
		return ret;
	}

	StructParser(SerLog* l, Filer* f, std::string name)
		: log(l)
		, filer(f)
		, struct_name(std::move(name))
	{
	}

	void complete()
	{
		if (!filer->is_loading)
			return;

		// check for specified but never read properties
		jsonh::Object* obj = get();
		for (const auto& [key, _] : obj->object)
		{
			if (names.find(key) == names.end())
			{
				const auto found = obj->object.find(key);
				const auto location
					= found == obj->object.end() ? obj->location : location_of(found->second, filer->doc, obj->location);
				log->errors.emplace_back(
					SerError{"property '" + key + "' in struct '" + struct_name + "' was never read", location.line, location.column}
				);
			}
		}
	}

	bool setup()
	{
		if (! filer->is_loading)
		{
			filer->json = filer->doc->add(jsonh::Object{});
			return true;
		}

		jsonh::Object* obj = filer->json.AsObject(filer->doc);
		if (obj == nullptr)
		{
			add_expected(log, "object for struct " + struct_name, filer);
			return false;
		}
		
		filer->json = filer->doc->add(*obj);
		return true;
	}
};

template<typename T>
void s_prop(StructParser* parser, const std::string& NAME, T* out)
{
	parser->names.emplace(NAME);
	if (parser->filer->is_loading)
	{
		auto* obj = parser->get();
		auto found = obj->object.find(NAME);
		if (found == obj->object.end())
		{
			parser->log->errors.emplace_back(SerError{"missing property " + NAME, obj->location.line, obj->location.column});
			return;
		}
		Filer ff{true, parser->filer->doc, found->second};
		ser(parser->log, &ff, out);
	}
	else
	{
		Filer ff{false, parser->filer->doc, {}};
		ser(parser->log, &ff, out);
		parser->get()->object.emplace(NAME, ff.json);
	}
}

template<typename T>
void s_prop_o(StructParser* parser, const std::string& NAME, std::optional<T>* out)
{
	parser->names.emplace(NAME);
	if (parser->filer->is_loading)
	{
		auto* obj = parser->get();
		auto found = obj->object.find(NAME);
		if (found == obj->object.end())
		{
			*out = std::nullopt;
			return;
		}
		Filer ff{true, parser->filer->doc, found->second};
		
		T temp;
		ser(parser->log, &ff, &temp);
		*out = temp;
	}
	else if (out->has_value())
	{
		Filer ff{false, parser->filer->doc, {}};
		T temp = out->value();
		ser(parser->log, &ff, &temp);
		parser->get()->object.emplace(NAME, ff.json);
	}
}

template<typename T>
void s_prop_v(StructParser* parser, const std::string& NAME, std::vector<T>* out)
{
	parser->names.emplace(NAME);
	if (parser->filer->is_loading)
	{
		out->resize(0);
		auto* obj = parser->get();
		auto found = obj->object.find(NAME);
		if (found == obj->object.end())
		{
			parser->log->errors.emplace_back(
				SerError{"missing array property " + NAME, obj->location.line, obj->location.column}
			);
			return;
		}

		auto arr_val = found->second;
		auto* arr = arr_val.AsArray(parser->filer->doc);
		if (arr == nullptr)
		{
			add_expected(parser->log, "array", arr_val, parser->filer->doc);
			return;
		}

		for (const auto& item: arr->array)
		{
			Filer ff{true, parser->filer->doc, item};
			T v;
			ser(parser->log, &ff, &v);
			out->emplace_back(v);
		}
	}
	else
	{
		jsonh::Value ret = parser->filer->doc->add(jsonh::Array());
		for (auto& s: *out)
		{
			Filer ff{false, parser->filer->doc, {}};
			ser(parser->log, &ff, &s);
			ret.AsArray(parser->filer->doc)->array.emplace_back(ff.json);
		}
		parser->get()->object.emplace(NAME, ret);
	}
}

template<typename T>
void s_prop_v(StructParser* parser, const std::string& NAME, std::unordered_map<wxString, T>* out)
{
	parser->names.emplace(NAME);
	if (parser->filer->is_loading)
	{
		auto* obj = parser->get();
		auto found = obj->object.find(NAME);
		if (found == obj->object.end())
		{
			parser->log->errors.emplace_back(
				SerError{"missing object property " + NAME, obj->location.line, obj->location.column}
			);
			return;
		}

		auto arr_val = found->second;
		auto* arr = arr_val.AsObject(parser->filer->doc);
		if (arr == nullptr)
		{
			add_expected(parser->log, "object", arr_val, parser->filer->doc);
			return;
		}

		const auto arrobj = arr->object;
		for (const auto& [key, item]: arrobj)
		{
			Filer ff{true, parser->filer->doc, item};
			T v;
			ser(parser->log, &ff, &v);
			out->emplace(key, v);
		}
	}
	else
	{
		jsonh::Value ret = parser->filer->doc->add(jsonh::Object());
		for (auto& [key, value]: *out)
		{
			Filer ff{false, parser->filer->doc, {}};
			ser(parser->log, &ff, &value);
			ret.AsObject(parser->filer->doc)->object.emplace(key, ff.json);
		}
		parser->get()->object.emplace(NAME, ret);
	}
}

#define F_STRUCT(STRUCT) void ser(SerLog* log, Filer* filer, ride::STRUCT* value)
#define S_BEGIN(STRUCT) \
	StructParser parser(log, filer, #STRUCT); \
	if (! parser.setup()) return
#define S_PROP(PROP, NAME) s_prop(&parser, NAME, &value->PROP)
#define S_PROP_O(PROP, NAME) s_prop_o(&parser, NAME, &value->PROP)
#define S_PROP_V(PROP, NAME) s_prop_v(&parser, NAME, &value->PROP)
#define S_END() parser.complete()

///////////////////////////////////////////////////////////////////////////////////////////////////
// forward declare all structs

F_STRUCT(Color);
F_STRUCT(Indicator);
F_STRUCT(FoldFlags);
F_STRUCT(Style);
F_STRUCT(FontsAndColors);
F_STRUCT(Theme);
F_STRUCT(WrapVisualFlags);
F_STRUCT(Settings);
F_STRUCT(OpenFile);
F_STRUCT(Session);
F_STRUCT(FindDlg);
F_STRUCT(DialogData);
F_STRUCT(BuildSetting);
F_STRUCT(Project);
F_STRUCT(RunSetting);
F_STRUCT(UserProject);
F_STRUCT(MachineSettings);

// ------------------------------------------------------------------------------------------------
// facade

std::string to_hex(int v)
{
	const char* digits = "0123456789abcdef";
	std::string result;
	result += digits[(v >> 4) & 0xF];
	result += digits[v & 0xF];
	return result;
}

void ser(SerLog* log, Filer* filer, ride::Color* value)
{
	if (filer->is_loading)
	{
		if (auto* str = filer->json.AsString(filer->doc); str != nullptr)
		{
			const auto color = str->value;
			if (color.size() != 7 || color[0] != '#')
			{
				// Handle invalid color format
				log->errors.emplace_back(
					SerError{"invalid color format", str->location.line, str->location.column}
				);
				return;
			}
			try
			{
				const auto r = std::stoi(color.substr(1, 2), nullptr, 16);
				const auto g = std::stoi(color.substr(3, 2), nullptr, 16);
				const auto b = std::stoi(color.substr(5, 2), nullptr, 16);
				value->r = r;
				value->g = g;
				value->b = b;
			}
			catch (const std::exception& x)
			{
				log->errors.emplace_back(
					SerError{std::string("invalid color format: ") + x.what(), str->location.line, str->location.column}
				);
			}
			return;
		}
		/// default handle
	}
	else
	{
		filer->json = filer->doc->add(
			jsonh::String{{}, "#" + to_hex(value->r) + to_hex(value->g) + to_hex(value->b)}
		);
		return;
	}
	S_BEGIN(Color);
	S_PROP(r, "r");
	S_PROP(g, "g");
	S_PROP(b, "b");
	S_END();
};

F_STRUCT(Indicator)
{
	S_BEGIN(Indicator);
	S_PROP(foreground, "foreground");
	S_PROP(under, "under");
	S_PROP(alpha, "alpha");
	S_PROP(outline_alpha, "outline_alpha");
	S_END();
};

F_STRUCT(FoldFlags)
{
	S_BEGIN(FoldFlags);
	S_PROP(LINEBEFORE_EXPANDED, "linebefore_expanded");
	S_PROP(LINEBEFORE_CONTRACTED, "linebefore_contracted");
	S_PROP(LINEAFTER_EXPANDED, "lineafter_expanded");
	S_PROP(LINEAFTER_CONTRACTED, "lineafter_contracted");
	S_PROP(LEVELNUMBERS, "levelnumbers");
	S_END();
};

F_STRUCT(Style)
{
	S_BEGIN(Style);
	S_PROP_O(typeface, "typeface");
	S_PROP_O(bold, "bold");
	S_PROP_O(italic, "italic");
	S_PROP_O(underline, "underline");
	S_PROP_O(font_size, "font_size");
	S_PROP_O(foreground, "foreground");
	S_PROP_O(background, "background");
	S_END();
};

// the idea of moving out all fonts and colors is that likely theese are
// what people want share: obsidian/zenburn/monokai and https://studiostyl.es/
F_STRUCT(FontsAndColors)
{
	S_BEGIN(FontsAndColors);

	S_PROP_V(colors, "colors");
	S_PROP_V(styles, "styles");
	S_PROP_V(alias, "alias");

	S_PROP(selected_line, "selected_line");
	S_PROP(fold_margin_hi, "fold_margin_hi");
	S_PROP(fold_margin_low, "fold_margin_low");
	S_PROP_O(selection_foreground, "selection_foreground");
	S_PROP_O(selection_background, "selection_background");
	S_PROP(edgeColor, "edgeColor");
	S_PROP(indicator_error, "indicator_error");
	S_PROP(indicator_warning, "indicator_warning");
	S_PROP(indicator_search_highlight, "indicator_search_highlight");
	S_PROP(indicator_select_highlight, "indicator_select_highlight");
	S_PROP(default_style, "default_style");
	
	S_PROP_O(line_number_style, "line_number_style");
	S_PROP_O(bracelight_style, "bracelight_style");
	S_PROP_O(bracebad_style, "bracebad_style");
	S_PROP_O(controlchar_style, "controlchar_style");
	S_PROP_O(indentguide_style, "indentguide_style");
	S_PROP_O(calltip_style, "calltip_style");
	S_PROP_O(annotation_warning_style, "annotation_warning_style");
	S_PROP_O(annotation_error_style, "annotation_error_style");

	S_PROP(explorer_foreground, "explorer_foreground");
	S_PROP(explorer_background, "explorer_background");

	S_PROP(marker_foreground, "marker_foreground");
	S_PROP(marker_background, "marker_background");
	S_PROP(caret_foreground, "caret_foreground");

	S_PROP(dock_background, "dock_background");
	S_PROP(dock_sash, "dock_sash");
	S_PROP(dock_active_caption, "dock_active_caption");
	S_PROP(dock_active_caption_gradient, "dock_active_caption_gradient");
	S_PROP(dock_inactive_caption, "dock_inactive_caption");
	S_PROP(dock_inactive_caption_gradient, "dock_inactive_caption_gradient");
	S_PROP(dock_active_caption_text, "dock_active_caption_text");
	S_PROP(dock_inactive_caption_text, "dock_inactive_caption_text");
	S_PROP(dock_border, "dock_border");
	S_PROP(dock_gripper, "dock_gripper");
	S_PROP(tab_background, "tab_background");
	S_PROP(tab_border, "tab_border");
	S_PROP(tab_sash, "tab_sash");
	S_PROP(tab_active_tab, "tab_active_tab");
	S_PROP(tab_inactive_tab, "tab_inactive_tab");
	S_PROP(tab_active_border, "tab_active_border");
	S_PROP(tab_inactive_border, "tab_inactive_border");
	S_PROP(tab_active_text, "tab_active_text");
	S_PROP(tab_inactive_text, "tab_inactive_text");
	S_PROP(statusbar_shadow, "statusbar_shadow");
	S_PROP(statusbar_highlight, "statusbar_highlight");
	S_PROP(statusbar_foreground, "statusbar_foreground");
	S_PROP(statusbar_background, "statusbar_background");
	S_PROP(switcher_background_color, "switcher_background_color");
	S_PROP(switcher_text_color, "switcher_text_color");
	S_PROP(switcher_selection_color, "switcher_selection_color");
	S_PROP(switcher_selection_outline_color, "switcher_selection_outline_color");
	S_PROP(switcher_dialog_color, "switcher_dialog_color");
	S_PROP(switcher_base_color, "switcher_base_color");

	S_END();
};

F_STRUCT(Theme)
{
	S_BEGIN(Theme);
	S_PROP(name, "name");
	S_PROP(can_remove, "can_remove");
	S_PROP(data, "data");
	S_END();
};

F_STRUCT(WrapVisualFlags)
{
	S_BEGIN(WrapVisualFlags);
	S_PROP(end, "end");
	S_PROP(start, "start");
	S_PROP(margin, "margin");
	S_END();
};

F_STRUCT(Settings)
{
	S_BEGIN(Settings);
	S_PROP(lineNumberEnable, "lineNumberEnable");
	S_PROP(statusbar_style, "statusbar_style");
	S_PROP(foldEnable, "foldEnable");
	S_PROP(displayEOL, "displayEOL");
	S_PROP(indentGuideEnable, "indentGuideEnable");
	S_PROP(whitespace, "whitespace");
	S_PROP(wordWrap, "wordWrap");
	S_PROP(edgeStyle, "edgeStyle");
	S_PROP(current_theme, "current_theme");
	S_PROP(edgeColumn, "edgeColumn");
	S_PROP(tabWidth, "tabWidth");
	S_PROP(useTabs, "useTabs");
	S_PROP(tabIndents, "tabIndents");
	S_PROP(backspaceUnindents, "backspaceUnindents");
	S_PROP(foldflags, "foldflags");
	S_PROP(virtual_space, "virtual_space");
	S_PROP(end_at_last_line, "end_at_last_line");
	S_PROP(vertical_scrollbar, "vertical_scrollbar");
	S_PROP(horizontal_scrollbar, "horizontal_scrollbar");
	S_PROP(wrap_visual_flags, "wrap_visual_flags");
	S_PROP(wrap_visual_flags_location, "wrap_visual_flags_location");
	S_PROP(wrap_indent_mode, "wrap_indent_mode");
	S_PROP(wrap_start_indent, "wrap_start_indent");
	S_PROP(annotations, "annotations");
	S_PROP(current_line_visible, "current_line_visible");
	S_PROP(current_line_alpha, "current_line_alpha");
	S_PROP(current_line_overdraw, "current_line_overdraw");
	S_PROP(highlight_word_also_highlight_keywords, "highlight_word_also_highlight_keywords");
	S_PROP(auto_indentation, "auto_indentation");
	// S_PROP(folderend, "folderend");
	// S_PROP(folderopenmid, "folderopenmid");
	// S_PROP(foldermidtail, "foldermidtail");
	// S_PROP(foldertail, "foldertail");
	// S_PROP(foldersub, "foldersub");
	// S_PROP(folder, "folder");
	// S_PROP(folderopen, "folderopen");
	S_PROP(autocomplete_curly_braces, "autocomplete_curly_braces");
	S_PROP(autocomplete_parentheses, "autocomplete_parentheses");
	S_PROP(autocomplete_brackets, "autocomplete_brackets");
	S_PROP(autocomplete_doublequote, "autocomplete_doublequote");
	S_PROP(autocomplete_singlequote, "autocomplete_singlequote");
	S_PROP(show_multiline_indicators, "show_multiline_indicators");
	S_PROP(show_compiler_messages_as_annotations, "show_compiler_messages_as_annotations");
	S_PROP_V(themes, "themes");
	S_PROP(caret_period, "caret_period");
	S_PROP(caret_width, "caret_width");
	S_PROP(caret_sticky, "caret_sticky");
	S_PROP(caret_style, "caret_style");
	S_PROP(home_end_style, "home_end_style");
	S_PROP(indicator_error, "indicator_error");
	S_PROP(indicator_warning, "indicator_warning");
	S_PROP(indicator_search_highlight, "indicator_search_highlight");
	S_PROP(indicator_select_highlight, "indicator_select_highlight");
	S_PROP(switcher_vs_focus, "switcher_vs_focus");
	S_PROP(switcher_row_count, "switcher_row_count");
	S_PROP(switcher_text_margin_x, "switcher_text_margin_x");
	S_PROP(switcher_text_margin_y, "switcher_text_margin_y");
	S_PROP(switcher_x_margin, "switcher_x_margin");
	S_PROP(switcher_y_margin, "switcher_y_margin");
	S_PROP(switcher_row_spacing, "switcher_row_spacing");
	S_PROP(switcher_col_spacing, "switcher_col_spacing");
	S_PROP(switcher_item_maxwidth, "switcher_item_maxwidth");
	S_PROP(switcher_item_maxheight, "switcher_item_maxheight");
	S_PROP(switcher_dlg_main_border, "switcher_dlg_main_border");
	S_PROP(switcher_dlg_item_border, "switcher_dlg_item_border");
	S_PROP(switcher_min_width, "switcher_min_width");
	S_PROP(switcher_min_height, "switcher_min_height");
	S_END();
};

/* ******************************************************************************************* */

F_STRUCT(OpenFile)
{
	S_BEGIN(OpenFile);
	S_PROP(path, "path");
 S_PROP(start_line, "start_line");
	S_PROP(start_index, "start_index");
	S_PROP(end_line, "end_line");
	S_PROP(end_index, "end_index");
	S_END();
};

F_STRUCT(Session)
{
	S_BEGIN(Session);
	S_PROP(window_x, "window_x");
	S_PROP(window_y, "window_y");
	S_PROP(window_width, "window_width");
	S_PROP(window_height, "window_height");
	S_PROP(state, "state");
	S_PROP(aui_perspective, "aui_perspective");
	S_END();
};

F_STRUCT(ProjectSession)
{
	S_BEGIN(ProjectSession)
	// todo(Gustav): add support for project explorer
	S_PROP_V(files, "files");
	S_END();
}

/* ******************************************************************************************* */


F_STRUCT(FindDlg)
{
	S_BEGIN(FindDlg);
	S_PROP(sub_folders, "sub_folders");
	S_PROP(match_case, "match_case");
	S_PROP(match_whole_word, "match_whole_word");
	S_PROP(match_start, "match_start");
	S_PROP(target, "target");
	S_PROP(file_types, "file_types");
	S_END();
};

F_STRUCT(DialogData)
{
	S_BEGIN(DialogData);
	S_PROP(find_dlg, "find_dlg");
	S_END();
};

/* ******************************************************************************************* */

F_STRUCT(BuildSetting)
{
	S_BEGIN(BuildSetting);
	S_PROP(name, "name");
	S_PROP(folder, "folder");
	S_PROP(build, "build");
	S_PROP(clean, "clean");
	S_END();
};

F_STRUCT(Project)
{
	S_BEGIN(Project);
	S_PROP(tabWidth, "tabWidth");
	S_PROP(useTabs, "useTabs");
	S_PROP_V(build_settings, "build_settings");
	S_END();
};

/* ******************************************************************************************* */

F_STRUCT(RunSetting)
{
	S_BEGIN(RunSetting);
	S_PROP(name, "name");
	S_PROP(application, "application");
	S_PROP(arguments, "arguments");
	S_PROP(folder, "folder");
	S_PROP(cmd_before_launch, "cmd_before_launch");
	S_PROP(wait_for_exit, "wait_for_exit");
	S_END();
};

F_STRUCT(UserProject)
{
	S_BEGIN(UserProject);
	S_PROP(build_setting, "build_setting");
	S_PROP(run_setting, "run_setting");
	S_PROP_V(run, "run");
	S_END();
};

/* ****************************************************************************************** */

F_STRUCT(MachineSettings)
{
	S_BEGIN(MachineSettings);
	S_PROP(cargo, "cargo");
	S_PROP(rustc, "rustc");
	S_PROP(protoc, "protoc");
	S_PROP(racer, "racer");
	S_PROP(rust_src_path, "rust_src_path");
	S_END();
};


// ------------------------------------------------------------------------------------------------
// facade

template<typename T>
wxString GenericLoad(SerLog* log, T* mess, const Fil& file)
{
	std::ifstream f(file.full_path().ToStdString());
	if(f.good() == false)
	{
		return "failed to open file";
	}

	std::ostringstream buffer;
	buffer << f.rdbuf();
	auto parsed = jsonh::Parse(buffer.str(), jsonh::parse_flags::IgnoreAllCommas);
	if (parsed.HasError())
	{
		for (const auto& err: parsed.errors)
		{
			log->errors.emplace_back(SerError{wxString::FromUTF8(err.message.c_str()), err.location.line, err.location.column});
		}
		return "Parsing failed";
	}

	auto filer = Filer{true, &parsed.doc, *parsed.root};
	ser(log, &filer, mess);
	return "";
}

template<typename T>
wxString GenericSave(T* mess, const Fil& file)
{
	jsonh::Document doc;
	// note: intentionally adding invalid object as that will be later overwritten
	auto filer = Filer{false, &doc, {}};
	SerLog log;
	ser(&log, &filer, mess);
	jsonh::Value root = filer.json;

	// make sure dir exist
	const auto dir = file.dir();
	if(dir.exist() == false)
	{
		if(false == dir.create())
		{
			return "failed to create dir: " + file.full_path();
		}
	}
	
	std::ofstream f(file.full_path().ToStdString());
	f << jsonh::Print(root, &doc, jsonh::print_flags::Json, jsonh::Pretty);
	if(!f.good())
	{
		return "failed to write file to " + file.full_path();
	}

	return "";
}

wxString SaveProtoJson(ride::UserProject* mess, const Fil& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::Theme* mess, const Fil& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::Settings* mess, const Fil& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::Session* mess, const Fil& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::Project* mess, const Fil& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::MachineSettings* mess, const Fil& file) { return GenericSave(mess, file); }
wxString SaveProtoJson(ride::ProjectSession* mess, const Fil& file) { return GenericSave(mess, file); }

wxString LoadProtoJson(SerLog* log, ride::UserProject* mess, const Fil& file) { return GenericLoad(log, mess, file); }
wxString LoadProtoJson(SerLog* log, ride::Theme* mess, const Fil& file) { return GenericLoad(log, mess, file); }
wxString LoadProtoJson(SerLog* log, ride::Settings* mess, const Fil& file) { return GenericLoad(log, mess, file); }
wxString LoadProtoJson(SerLog* log, ride::Session* mess, const Fil& file) { return GenericLoad(log, mess, file); }
wxString LoadProtoJson(SerLog* log, ride::Project* mess, const Fil& file) { return GenericLoad(log, mess, file); }
wxString LoadProtoJson(SerLog* log, ride::MachineSettings* mess, const Fil& file) { return GenericLoad(log, mess, file); }
wxString LoadProtoJson(SerLog* log, ride::ProjectSession* mess, const Fil& file) { return GenericLoad(log, mess, file); }












