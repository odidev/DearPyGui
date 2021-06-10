#include <utility>
#include "mvAnnotation.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
//#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvAnnotation::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("default_value", mvArgType::KEYWORD_ARG, "(0.0, 0.0)");
		parser.addArg<mvPyDataType::FloatList>("offset", mvArgType::KEYWORD_ARG, "(0.0, 0.0)");
		
		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");

		parser.addArg<mvPyDataType::Bool>("clamped", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvAnnotation::mvAnnotation(mvUUID uuid)
		: mvDouble4PtrBase(uuid)
	{
	}

	bool mvAnnotation::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvPlot)
			return true;

		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

	void mvAnnotation::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		//mvFontScope fscope(this);

		if (m_clamped)
			ImPlot::AnnotateClamped((*m_value.get())[0], (*m_value.get())[1], m_pixOffset, m_color.toVec4(), m_specificedlabel.c_str());
		else
			ImPlot::Annotate((*m_value.get())[0], (*m_value.get())[1], m_pixOffset, m_color.toVec4(), m_specificedlabel.c_str());

	}

	void mvAnnotation::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "clamped")) m_clamped = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "offset")) m_pixOffset = ToVec2(item);

	}

	void mvAnnotation::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "clamped", ToPyBool(m_clamped));
		PyDict_SetItemString(dict, "offset", ToPyPair(m_pixOffset.x, m_pixOffset.y));

	}

}