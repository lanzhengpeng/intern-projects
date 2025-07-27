### 【1454】inherit selected feature data to drawing 继承选定特征数据至图纸

#### 代码

```cpp
    /*HEAD INHERIT_SELECTED_FEATURE_DATA_TO_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static tag_t select_a_view(char *prompt)  
    {  
        int  
            view_of_cursor,  
            resp;  
        double  
            pos[3];  
        char  
            vname[MAX_ENTITY_NAME_SIZE+1];  
        tag_t  
            view = NULL_TAG;  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( 0));  
        resp = uc1615(prompt, pos);  
        UF_CALL( UF_UI_set_cursor_view( view_of_cursor));  
        if (resp == 5)  
        {  
            UF_CALL( uc1653( vname ));  
            UF_CALL( UF_VIEW_ask_tag_of_view_name(vname, &view));  
            return view;  
        }  
        else  
            return NULL_TAG;  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat[1],  
            occs[1] = { NULL_TAG },  
            view[1];  
        char  
            *annotation_template_name;  
        while (((view[0] = select_a_view("Inherit data to")) != NULL_TAG) &&  
               ((feat[0] = select_a_feature("Inherit data from")) != NULL_TAG))  
        {  
            UF_CALL(UF_DRF_ask_annotation_template(&annotation_template_name));  
            if (strcmp(annotation_template_name, "ansi"))  
                UF_CALL(UF_DRF_set_annotation_template("ansi"));  
            UF_free(annotation_template_name);  
            UF_CALL(UF_DRF_inherit_feature_data(1, feat, occs, 1, view,  
                UF_DRF_INHERIT_FEATURE_PARAMETERS));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码，这是一段NX的二次开发代码，主要实现的功能是：
>
> 1. 选择视图：通过调用UF_UI_ask_cursor_view()和UF_UI_set_cursor_view()，让用户选择一个视图，并将其设置为当前视图。
> 2. 选择特征：通过调用UF_UI_select_feature()，让用户选择一个特征。
> 3. 数据继承：如果用户选择了视图和特征，代码会调用UF_DRF_inherit_feature_data()，将特征的数据（包括参数）继承到选定的视图上。
> 4. 设置注释模板：在继承特征数据前，调用UF_DRF_ask_annotation_template()和UF_DRF_set_annotation_template()，设置注释模板为ANSI格式。
> 5. 错误处理：使用宏ECHO()和report_error()，在函数执行出错时输出错误信息。
> 6. 初始化与终止：在ufusr()函数中，通过UF_initialize()和UF_terminate()，完成NX的初始化和终止。
> 7. 卸载请求：通过ufusr_ask_unload()，设置立即卸载二次开发代码。
>
> 总体来看，这段代码实现了在NX中让用户选择视图和特征，然后继承特征数据到视图上的功能，包括一些错误处理和初始化/终止NX的调用。
>
