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
        // TODO i dont think this works when i dont have DevTools
        if (getID() != "portal-tab-bar")
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

        auto newBtn = g_editorUi->getCreateBtn(BigPortal::OBJECT_ID, 4);

        // try to insert it after the mini portal
        bool inserted = false;
        for (int i = 0; i < objects->count(); i++)
        {
            auto btn = static_cast<CreateMenuItem *>(objects->objectAtIndex(i));
            if (btn->m_objectID == 101)
            {
                objects->insertObject(newBtn, i + 1);
                inserted = true;
                break;
            }
        }

        // if it fails for some reason, put it at the end
        if (!inserted)
            objects->addObject(newBtn);

        EditButtonBar::loadFromItems(objects, rows, columns, keepPage);
    }
};
