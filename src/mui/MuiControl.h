/* Copyright 2012 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

#ifndef MuiControl_h
#define MuiControl_h

// This is only meant to be included by Mui.h inside mui namespace

class Layout;
class EventMgr;

class Control
{
public:
    // allows a window to opt-out from being notified about
    // input events, stored in wantedInputBits
    enum WantedInputBits : int {
        WantsMouseOverBit   = 0,
        WantsMouseDownBit   = 1,
        WantsMouseUpBit     = 2,
        WantsMouseClickBit  = 3,
        WantsMouseMoveBit   = 4,
        WantedInputBitLast
    };

    // describes current state of a window, stored in stateBits
    enum ControlStateBits : int {
        MouseOverBit = 0,
        IsPressedBit = 1,
        // using IsHidden and not IsVisible so that 0 is default, visible state
        IsHiddenBit  = 2,
        StateBitLast
    };

    Control(Control *newParent=NULL);
    virtual ~Control();

    void        SetParent(Control *newParent);
    void        AddChild(Control *wnd, int pos = -1);
    Control *   GetChild(size_t idx) const;
    size_t      GetChildCount() const;

    void        SetPosition(const Rect& p);

    virtual void Paint(Graphics *gfx, int offX, int offY);

    // WPF-like layout system. Measure() should update desiredSize
    // Then the parent uses it to calculate the size of its children
    // and uses Arrange() to set it.

    // availableSize can have SizeInfinite as dx or dy to allow
    // using as much space as the window wants
    virtual void Measure(const Size availableSize);
    virtual void Arrange(const Rect finalRect);

    // mouse enter/leave are used e.g. by a button to change the look when mouse
    // is over them. The intention is that in response to those a window should
    // only do minimal processing that affects the window itself, not the rest
    // of the system
    virtual void NotifyMouseEnter() {}
    virtual void NotifyMouseLeave() {}

    virtual void NotifyMouseMove(int x, int y) {}

    virtual void RegisterEventHandlers(EventMgr *evtMgr) {}
    virtual void UnRegisterEventHandlers(EventMgr *evtMgr) {}

    bool WantsMouseClick() const;
    bool WantsMouseMove() const;
    bool IsMouseOver() const;
    void SetIsMouseOver(bool isOver);

    bool IsVisible() const;
    void Hide();
    void Show();

    void MeasureChildren(Size availableSize) const;
    void MapRootToMyPos(int& x, int& y) const;

    uint16          wantedInputBits; // WndWantedInputBits
    uint16          stateBits;       // WndStateBits
    // windows with bigger z-order are painted on top, 0 is default
    int16           zOrder;

    Layout *        layout;
    Control *       parent;

    // we cache properties for the current style during SetStyle() which
    // makes if fast to access them anywhere without repeating the work
    // of searching the style inheritance chain
    // properties are cached in a separate cache and not in each object
    // as it allows us to save per-object memory by sharing those properties
    // (we expect many objects of the same type to have the same style).
    // For now we cache every prop for each object type. We could limit
    // the set of cached props per-object (but right now it doesn't seem
    // to be enoug of a saving)
    Prop **         cachedProps;
    Prop **         GetCachedProps() const;
    Prop *          GetCachedProp(PropType propType) const;
    void            SetCurrentStyle(Style *style1, Style *style2);

    // only used by HwndWrapper but we need it here
    HWND            hwndParent;

    // cursor to show when mouse is over this window.
    // only works if the window sets WantsMouseOverBit.
    // Control doesn't own hCursor in order to enable easy
    // sharing of cursor among many windows.
    HCURSOR         hCursor;

    // position and size (relative to parent, might be outside of parent's bounds)
    Rect            pos;

    // desired size calculated in Measure()
    Size            desiredSize;

private:
    Vec<Control*>   children;
};

#endif