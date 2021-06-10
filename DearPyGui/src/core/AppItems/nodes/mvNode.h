#pragma once
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>
#include <imnodes.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvNode, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvNode : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		friend class mvNodeEditor;

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvNode, add_node)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS

		MV_END_STYLE_CONSTANTS

	public:

		mvNode(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		bool canChildBeAdded(mvAppItemType type) override;

		int getId() const {return m_id;}

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

	    int m_id = 0;
		bool m_draggable = true;
	};

}