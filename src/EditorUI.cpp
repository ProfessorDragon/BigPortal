#include <Geode/Geode.hpp>
#include "BigPortal/BigPortal.hpp"

using namespace geode::prelude;

static EditorUI *g_editorUi;

#include <Geode/modify/EditorUI.hpp>
class $modify(EditorUI)
{
    bool init(LevelEditorLayer *editorLayer)
    {
        // hack
        // https://github.com/NicknameGG/The-intense-adding-of-a-new-object-in-a-new-tab
        g_editorUi = this;

        return EditorUI::init(editorLayer);
    }
};

#include <Geode/modify/EditButtonBar.hpp>
class $modify(EditButtonBar)
{
    void loadFromItems(CCArray *objects, int rows, int columns, bool keepPage)
    {
        // getID() != "portal-tab-bar" // doesn't work on some devices
        if (m_tabIndex != 5)
        {
            EditButtonBar::loadFromItems(objects, rows, columns, keepPage);
            return;
        }

        if (!g_editorUi)
        {
            log::error("EditorUI is null!");
            EditButtonBar::loadFromItems(objects, rows, columns, keepPage);
            return;
        }

        // insert portal into objects array
        int insertIndex = objects->count();

        for (int i = 0; i < objects->count(); i++)
        {
            auto btn = static_cast<CreateMenuItem *>(objects->objectAtIndex(i));

            // try to insert it after the mini portal
            if (btn->m_objectID == 101)
            {
                insertIndex = i + 1;
            }
            // big portal has aleady been added, abort
            else if (btn->m_objectID == BigPortal::OBJECT_ID)
            {
                EditButtonBar::loadFromItems(objects, rows, columns, keepPage);
                return;
            }
        }

        // insertIndex will fall back to the end of the array
        if (auto newBtn = g_editorUi->getCreateBtn(BigPortal::OBJECT_ID, 4))
        {
            objects->insertObject(newBtn, insertIndex);
        }

        EditButtonBar::loadFromItems(objects, rows, columns, keepPage);
    }
};
