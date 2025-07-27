### 【2961】test uguidialog with real and integer scales 测试使用实数和整数刻度的用户界面对话框

#### 代码

```cpp
    /*HEAD TEST_UGUIDIALOG_WITH_REAL_AND_INTEGER_SCALES.CXX CCC UFUN */  
    /* To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "GTAC"  
        - Specify the Constructor Callback as GTAC"const_cb"  
        - Specify the Destructor Callback as GTAC"destr_cb"  
        - Add an Integer Scale  
        - Add a Real Scale  
        - Save Dialog As "application/gtac"  
    */  
    #include <stdio.h>  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_ui_dialog.hxx>  
    #include <ug_ui_integer_scale.hxx>  
    #include <ug_ui_real_scale.hxx>  
    #include <uf.h>  
    #include <uf_styler.h>  
    #define GTAC_SCALE_INT_0               ("SCALE_INT_0")  
    #define GTAC_SCALE_REAL_1              ("SCALE_REAL_1")  
    #define ARRAY_LENGTH( array ) ( sizeof( array ) / sizeof( array[0] ) )  
    class GTACdialog: public UgUIDialog  
    {  
        public:  
            static GTACdialog *getInstance(void);  
            static void invalidateInstance(void);  
            void initializeThis(void);  
            void sayValues(void);  
        private:  
            GTACdialog(void);  
            virtual ~GTACdialog(void);  
            static GTACdialog *m_instance;  
            UgUIIntegerScale *m_Iscale;  
            UgUIRealScale *m_Rscale;  
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
        UF_print_syslog("* initializing scale test UgUIdialog\n", FALSE);  
        m_Iscale->setValue(2);  
        m_Rscale->setValue(3.5);  
    }  
    void GTACdialog::sayValues( void )  
    {  
        char  
            msg[133];  
        UgInfoWindow  
            info;  
        info.open();  
        sprintf(msg, "Integer value is %d\n", m_Iscale->getValue());  
        info.write(msg);  
    /*  This does not return the correct number - see PR 1286796 里海译:这并不返回正确的数字 - 请看 PR 1286796 */  
        sprintf(msg, "Real value is %.1f - NOT!\n", m_Rscale->getValue());  
        info.write(msg);  
    }  
    GTACdialog::GTACdialog(void)  
    {  
        UF_print_syslog("* constructing scale test UgUIdialog\n", FALSE);  
        m_Iscale = new UgUIIntegerScale(GTAC_SCALE_INT_0, this);  
        m_Rscale = new UgUIRealScale(GTAC_SCALE_REAL_1, this);  
    }  
    GTACdialog::~GTACdialog(void)  
    {  
        UF_print_syslog("* destructing scale test UgUIdialog\n", FALSE);  
        delete m_Iscale;  
        delete m_Rscale;  
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
        char  
            msg[133];  
        UF_STYLER_item_value_type_t  
            ask_value;  
        GTACdialog::getInstance()->sayValues();  
    /*  This is the work around for PR 1286796 里海译:对于 PR 1286796 的解决方法，译文如下：这是针对 PR 1286796 的解决方法。 */  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id =  GTAC_SCALE_REAL_1;  
        UF_STYLER_ask_value(dialog_id, &ask_value);  
        sprintf(msg, "The Real value IS %.1f\n", ask_value.value.real);  
        UgInfoWindow::write(msg);  
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
        UgException::throwOnFailure(UF_STYLER_create_dialog("gtac.dlg",  
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

> 这段代码是一个NX的二次开发示例，其主要功能是创建一个包含整数尺度和实数尺度的对话框，并在其中显示和获取这两个尺度的值。
>
> 主要思路包括：
>
> 1. 定义GTACdialog类，继承自UgUIDialog，用于封装对话框的逻辑。
> 2. 在GTACdialog中，创建整数尺度和实数尺度对象，并设置其初始值。
> 3. 实现构造函数和析构函数，用于初始化和清理尺度对象。
> 4. 提供方法用于显示尺度值。
> 5. 定义静态方法，用于获取GTACdialog单例。
> 6. 定义回调函数，用于初始化对话框、销毁对话框和应用对话框。
> 7. 在ufusr函数中，创建对话框，并调用do_it函数，用于显示对话框并获取用户输入。
> 8. 在回调函数中，使用GTACdialog单例进行尺度值的获取和设置。
>
> 通过这个示例，展示了如何在NX中创建和使用自定义对话框，以及如何通过回调函数处理对话框事件。
>
