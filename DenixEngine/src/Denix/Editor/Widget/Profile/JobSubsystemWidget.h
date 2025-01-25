#pragma once
#include "Denix/Editor/EditorWidget.h"

namespace Denix
{
    /**
     * \brief JobSubsystemWidget class for displaying and managing the job subsystem in the editor.
     */
    class JobSubsystemWidget: public EditorWidget
    {
    public:
        /**
         * \brief Constructor for JobSubsystemWidget.
         * \param _objInit Initialization parameters for the object.
         */
        JobSubsystemWidget(const ObjectInit& _objInit)
            : EditorWidget(_objInit)
        {
        }

        /**
         * \brief Updates the job subsystem widget.
         * \param _deltaTime Time elapsed since the last update.
         */
        void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
    };
}