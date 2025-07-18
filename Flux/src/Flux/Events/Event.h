#pragma once


#include "fxpch.h"
#include "Flux/Core.h"


namespace Flux 
{

	// Events in Flux are handled immediately and block execution.
    // In the future, buffering events in an event bus for later processing
    // during the update phase might be better.

    enum class EventType
    {
        None = 0, 
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput       = BIT(1),
        EventCategoryKeyboard    = BIT(2),
        EventCategoryMouse       = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

    #define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								   virtual EventType GetEventType() const override { return GetStaticType(); }\
								   virtual const char* GetName() const override { return #type; }

   #define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class FLUX_API Event
    {
        friend class EventDispatcher;
    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory _category)
        {
            return GetCategoryFlags() & _category;
        }

        bool IsHandled() const { return m_Handled; }
   
    protected:
        bool m_Handled = false; //Input doesn't need to pass through alle layers (e.g. Button clicked -> Event was handled and Layer underneath it does not need to receive MouseButtonEvent)
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& _event)
            : m_Event(_event)
        {
        }

        template<typename T>
        bool Dispatch(EventFn<T> _func) 
        {
            if (m_Event.GetEventType() == T::GetStaticType()) 
            {
                m_Event.m_Handled = _func(*(T*)&m_Event);
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };

    inline std::string format_as(const Event& e) {
        return e.ToString();
    }
}