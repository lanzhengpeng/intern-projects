### 【0650】create constrained pattern array 创建约束模式阵列

#### 代码

```cpp
    /*****************************************************************************  
    ** Works also with new NX8 Pattern Feature  
    *****************************************************************************/  
    /* Include files 译:Include files的翻译为：包含文件 */  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
    #include <stdio.h>  
    #include <stdarg.h>  
    static void ECHO(char* format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char* file, int line, char* call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static int mask_for_components(UF_UI_selection_p_t select, void* type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char* prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_a_feature(char* prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            * feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr(char* parm, int* returnCode, int rlen)  
    {  
        /* Initialize the API environment 译:初始化API环境 */  
        if (UF_CALL(UF_initialize()))  
        {  
            /* Failed to initialize 译:Failed to initialize 的中文意思是“初始化失败”。 */  
            return;  
        }  
        /* TODO: Add your application code here 译:翻译：TODO: 在这里添加你的应用程序代码 */  
        tag_t  
            comp = NULL_TAG,  
            feat = NULL_TAG,  
            iset = NULL_TAG;  
        UF_ASSEM_iset_array_data_t  
            array_data;  
        int  
            dimensions[UF_ASSEM_max_dim_count];  
        dimensions[0] = 3;  
        dimensions[1] = NULL;  
        if ((comp = select_a_component("Select Component")) != NULL_TAG &&  
            (feat = select_a_feature("Select Pattern Feature")) != NULL_TAG)  
        {  
            array_data.array_name = "myarray";  
            array_data.template_component = comp;  
            array_data.feature_iset = feat;  
            array_data.dimensions[0] = 3;  
            array_data.dimensions[1] = NULL;  
            UF_CALL(UF_ASSEM_create_constrained_iset_array(&array_data, &iset));  
        }  
        /* Terminate the API environment 译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload(void)  
    {  
        return(UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码内容，这是一段用于在NX中创建约束阵列组件的二次开发代码。具体功能如下：
>
> 1. 代码概述：首先引入了NX的UF库函数头文件，定义了一些宏和错误处理函数。
> 2. 组件选择：使用UF_UI_select_with_single_dialog函数选择组件，并使用自定义的mask_for_components函数来限制只选择组件。
> 3. 特征选择：使用UF_UI_select_feature函数选择特征。
> 4. 创建约束阵列：如果选择了组件和特征，使用UF_ASSEM_create_constrained_iset_array函数创建约束阵列。
> 5. 用户函数：定义了ufusr函数，用于初始化UF环境，选择组件和特征，创建约束阵列，然后终止UF环境。
> 6. 卸载处理：定义了ufusr_ask_unload函数，用于卸载用户函数时返回立即卸载。
> 7. 代码功能：整体上，这段代码实现了在NX中通过UI选择组件和特征，然后创建约束阵列的功能，方便用户进行参数化建模。
> 8. 使用说明：用户可以通过NX的“执行用户函数”菜单调用这段代码，实现约束阵列的快速创建。
>
