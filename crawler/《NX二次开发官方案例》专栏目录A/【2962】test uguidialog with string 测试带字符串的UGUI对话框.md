### 【2962】test uguidialog with string 测试带字符串的UGUI对话框

#### 代码

```cpp
    /*HEAD TEST_UGUIDIALOG_WITH_STRING.CXX CCC UFUN */  
    /* To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "GTAC"  
        - Specify the Constructor Callback as GTAC"const_cb"  
        - Specify the Destructor Callback as GTAC"destr_cb"  
        - Add a String  
        - Save Dialog As "application/string"  
    */  
    #include <stdio.h>  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_ui_dialog.hxx>  
    #include <ug_ui_string.hxx>  
    #include <uf.h>  
    #include <uf_styler.h>  
    #define GTAC_STR_0                     ("STR_0")  
    #define ARRAY_LENGTH( array ) ( sizeof( array ) / sizeof( array[0] ) )  
    class GTACdialog: public UgUIDialog  
    {  
        public:  
            static GTACdialog *getInstance(void);  
            static void invalidateInstance(void);  
            void initializeThis(void);  
            void sayValue(void);  
        private:  
            GTACdialog(void);  
            virtual ~GTACdialog(void);  
            static GTACdialog *m_instance;  
            UgUIString *m_String;  
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
        UF_print_syslog("* initializing string test UgUIdialog\n", FALSE);  
        std::string input;  
        input = "Default";  
        m_String->setValue(input);  
    }  
    void GTACdialog::sayValue( void )  
    {  
        UgInfoWindow  
            info;  
        info.open();  
        info.write(m_String->getValue());  
        info.write("\n");  
    }  
    GTACdialog::GTACdialog(void)  
    {  
        UF_print_syslog("* constructing string test UgUIdialog\n", FALSE);  
        m_String = new UgUIString(GTAC_STR_0, this);  
    }  
    GTACdialog::~GTACdialog(void)  
    {  
        UF_print_syslog("* destructing tring test UgUIdialog\n", FALSE);  
        delete m_String;  
    }  
    static int GTAC_const_cb( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        GTACdialog *dialog = GTACdialog::getInstance();  
        dialog->initialize( dialog_id );  
        return( UF_UI_CB_CONTINUE_DIALOG );  
    }  
    static int GTAC_destr_cb( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        GTACdialog::getInstance()->invalidate();  
        GTACdialog::invalidateInstance();  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static int GTAC_apply_cb( int dialog_id, void * client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        GTACdialog::getInstance()->sayValue();  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[] =  
            {  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_DESTRUCTOR_CB , 0, GTAC_destr_cb},  
             {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB, 0, GTAC_apply_cb},  
             {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
            };  
        UgException::throwOnFailure(UF_STYLER_create_dialog("string.dlg",  
            GTAC_cbs, ARRAY_LENGTH(GTAC_cbs), NULL, &resp));  
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

> 这段代码是一个基于NX Open UI的二次开发示例，用于演示如何创建和使用一个简单的用户界面对话框。
>
> 主要功能包括：
>
> 1. 创建一个名为GTACdialog的对话框类，继承自UgUIDialog类。该类包含一个字符串成员变量，用于在对话框中显示和编辑文本。
> 2. 提供了getInstance和invalidateInstance两个静态方法，用于获取和销毁GTACdialog的实例。
> 3. 在GTACdialog的构造函数中，创建了一个UgUIString对象，并将其添加到对话框中。在析构函数中，删除这个字符串对象。
> 4. 提供了initializeThis方法，用于初始化对话框，将字符串设为默认值。
> 5. 提供了sayValue方法，用于在信息窗口中显示字符串的当前值。
> 6. 定义了三个回调函数GTAC_const_cb、GTAC_destr_cb和GTAC_apply_cb，用于在对话框构造、析构和应用时执行相应操作。
> 7. 在主函数ufusr中，创建了一个UgSession对象，并调用do_it函数创建和显示对话框。
> 8. do_it函数使用UF_STYLER_create_dialog函数创建对话框，并指定了三个回调函数。
>
> 总的来说，这段代码演示了如何使用NX Open UI创建一个简单的对话框，并在其中添加字符串控件，以及如何处理对话框的生命周期事件。
>
