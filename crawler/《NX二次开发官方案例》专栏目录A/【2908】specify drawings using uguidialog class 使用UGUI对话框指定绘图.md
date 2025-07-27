### 【2908】specify drawings using uguidialog class 使用UGUI对话框指定绘图

#### 代码

```cpp
    /*HEAD SPECIFY_DRAWINGS_USING_UGUIDIALOG_CLASS.CPP CCC UFUN */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Set the dialog Title as "Specify Drawings"  
        - Change the dialog's Prefix Name to "GTAC"  
        - Set the Button Style Options to OK and Cancel  
        - Specify the Constructor Callback as GTAC"const_cb"  
        - Specify the Destructor Callback as GTAC"destr_cb"  
        - Specify the OK Callback as GTAC"ok_cb"  
        - Add a Multi Select List object  
        - Set its Number of Visible Items to 10  
        - Add a Push Button  
        - Change its Label to "Select All"  
        - Save Dialog As "application/drawings"  
    */  
    #include <vector>  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_typed.hxx>  
    #include <ug_ui_dialog.hxx>  
    #include <ug_ui_multi_select_list.hxx>  
    #include <ug_ui_action_button.hxx>  
    #include <uf.h>  
    #include <uf_styler.h>  
    #include <uf_draw.h>  
    #define GTAC_MULTI_LIST_0              ("MULTI_LIST_0")  
    #define GTAC_ACTION_1                  ("ACTION_1")  
    #define ARRAY_LENGTH( array ) ( sizeof( array ) / sizeof( array[0] ) )  
    class GTACdialog: public UgUIDialog  
    {  
        public:  
            static GTACdialog *getInstance(void);  
            static void invalidateInstance(void);  
            void initializeThis(void);  
            void getDrawings(std::vector< UgTypedObject *> *);  
            void selectAll(void);  
        private:  
            GTACdialog(void);  
            virtual ~GTACdialog(void);  
            static GTACdialog *m_instance;  
            UgUIMultiSelectList *m_dwgList;  
    };  
    GTACdialog *GTACdialog::m_instance = NULL;  
    GTACdialog *GTACdialog::getInstance(void)  
    {  
        if ( m_instance == NULL )  
        {  
            m_instance = new GTACdialog;  
        }  
        return m_instance;  
    }  
    void GTACdialog::invalidateInstance(void)  
    {  
        if ( m_instance != NULL )  
        {  
            delete m_instance;  
            m_instance = NULL;  
        }  
    }  
    void GTACdialog::initializeThis( void )  
    {  
        int  
            dft = 0,  
            n;  
        tag_t  
            *drawings;  
        tag_t  
            drawing;  
        UF_print_syslog("* initializing drawing selection UgUIdialog\n", FALSE);  
        UF_DRAW_ask_current_drawing(&drawing);  
        UF_DRAW_ask_drawings(&n, &drawings);  
        if (n > 0)  
        {  
            for (int ii = 0; ii < n; ii++)  
            {  
                UgTypedObject *a_dwg = dynamic_cast < UgTypedObject *>  
                    ( UgObject::find(drawings[ii]) );  
                m_dwgList->insert(a_dwg->getName(), ii);  
                if (drawings[ii] == drawing) dft = ii;  
            }  
            UF_free(drawings);  
            m_dwgList->select(dft);  
            if (dft >= 10) m_dwgList->show(dft-9);  
        }  
        else  
        {  
            m_dwgList->insert("<No Drawings Found>", 0);  
            m_dwgList->select(0);  
        }  
    }  
    void GTACdialog::getDrawings(std::vector< UgTypedObject *> *drawings)  
    {  
        UgTypedObject  
            *a_dwg;  
        int  
            ii,  
            n;  
        tag_t  
            *drawing_tags;  
        std::vector< int >  
            picks = m_dwgList->askSelectedIndices();  
        UF_DRAW_ask_drawings(&n, &drawing_tags);  
        if (n > 0)  
        {  
            for (ii = 0; ii < picks.size(); ii++)  
            {  
                a_dwg = dynamic_cast < UgTypedObject *>  
                    ( UgObject::find(drawing_tags[picks[ii]]) );  
                drawings->push_back(a_dwg);  
            }  
            UF_free(drawing_tags);  
        }  
    }  
    void GTACdialog::selectAll(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *drawing_tags;  
        std::vector< int >  
            picks = m_dwgList->askSelectedIndices();  
    //  the "select" method is a toggle - so toggle ons off first  
        for (ii = 0; ii < picks.size(); ii++) m_dwgList->select(picks[ii]);  
    //  then turn everything on  
        UF_DRAW_ask_drawings(&n, &drawing_tags);  
        m_dwgList->select(0);  
        for (ii = 1; ii < n; ii++) m_dwgList->select(ii);  
        if (n > 0) UF_free(drawing_tags);  
    }  
    GTACdialog::GTACdialog(void)  
    {  
        UF_print_syslog("* constructing drawing selection UgUIdialog\n", FALSE);  
        m_dwgList = new UgUIMultiSelectList(GTAC_MULTI_LIST_0, this);  
    }  
    GTACdialog::~GTACdialog(void)  
    {  
        UF_print_syslog("* destructing drawing selection UgUIdialog\n", FALSE);  
        delete m_dwgList;  
    }  
    static int GTAC_const_cb( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        GTACdialog *dialog = GTACdialog::getInstance();  
        dialog->initialize( dialog_id );  
        return( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static int GTAC_ok_cb( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        std::vector< UgTypedObject *> *drawings =  
            static_cast <std::vector< UgTypedObject *> *> (client_data);  
        GTACdialog::getInstance()->getDrawings(drawings);  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    static int GTAC_action_1_act_cb( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        GTACdialog::getInstance()->selectAll();  
        return( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static int GTAC_destr_cb( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        GTACdialog::getInstance()->invalidate();  
        GTACdialog::invalidateInstance();  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        std::vector< UgTypedObject *>  
            drawings;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[] =  
            {  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_DESTRUCTOR_CB , 0, GTAC_destr_cb},  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB, 0, GTAC_ok_cb},  
             {GTAC_ACTION_1, UF_STYLER_ACTIVATE_CB, 0, GTAC_action_1_act_cb},  
             {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
            };  
        do  
        {  
            UgException::throwOnFailure(UF_STYLER_create_dialog(  
                "specify_drawings.dlg", GTAC_cbs, ARRAY_LENGTH(GTAC_cbs),  
                &drawings, &resp));  
            if (resp == UF_UI_OK)  
            {  
                UgInfoWindow  info;  
                info.open();  
                for (int ii = 0; ii < drawings.size(); ii++)  
                {  
                    info.write(drawings[ii]->getName());  
                    info.write("\n");  
                }  
                drawings.clear();  
            }  
        } while (resp == UF_UI_OK);  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        UgSession session(TRUE);  
        try  
        {  
            do_it();  
        }  
        catch (UgException oops)  
        {  
            oops.processError();  
        }  
    }

```

#### 代码解析

> 根据提供的代码，这是一个NX的二次开发示例，主要功能是创建一个对话框，用于选择当前NX会话中的绘图，并将所选绘图的名称显示在信息窗口中。具体功能如下：
>
> 1. 定义了一个GTACdialog类，继承自UgUIDialog。该类包含一个多选列表，用于显示当前NX会话中的所有绘图，并提供全选和获取所选绘图的方法。
> 2. 创建对话框时，会调用GTACdialog的initializeThis方法，获取当前NX会话中的所有绘图，并将它们显示在多选列表中。当前绘图会被默认选中。
> 3. 点击确定按钮时，会调用GTACdialog的getDrawings方法，获取用户选中的绘图，并将它们的名称显示在信息窗口中。
> 4. 点击全选按钮时，会调用GTACdialog的selectAll方法，将所有绘图选中。
> 5. 对话框销毁时，会调用GTACdialog的invalidate方法，清理相关资源。
> 6. 在ufusr函数中，会创建GTACdialog对话框，并在一个循环中处理对话框的返回值。当用户点击确定时，会获取所选绘图并显示，然后继续循环。当用户点击取消时，会退出循环。
> 7. 使用try-catch语句捕获并处理可能发生的异常。
>
> 总的来说，这段代码实现了在NX中创建一个简单的绘图选择对话框，并将所选绘图名称显示在信息窗口中的功能。
>
