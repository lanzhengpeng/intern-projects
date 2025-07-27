### 【1007】delete all explosions 删除所有爆炸

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** delete_all_explosions.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 里海译:翻译：Include files */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_layout.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
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
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_modeling_views(tag_t part, tag_t **views)  
    {  
        tag_t  
            view = NULL_TAG;  
        uf_list_p_t  
            view_list;  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_CALL(UF_MODL_create_list(&view_list));  
        while ((view = ask_next_view(part, view)) != NULL_TAG)  
        {  
            UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
            if (type == UF_VIEW_MODEL_TYPE)  
                UF_CALL(UF_MODL_put_list_item(view_list, view));  
        }  
        return (make_an_array(&view_list, views));  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        tag_t   
            part_tag = NULL_TAG,  
            *explosion_tags,  
            *views,  
            work_view = NULL_TAG;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        int   
            ii,  
            n_explosions = 0,  
            n_views = 0;  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 里海译:Failed to initialize 翻译为：初始化失败。 */  
            return;  
        }  
        // get displayed part  
        part_tag = UF_PART_ask_display_part();  
        if( part_tag == NULL_TAG )  
        {  
            ECHO("Error: There is no displayed part yet!\n");  
            return;  
        }  
        // backup current work view  
        UF_CALL( UF_VIEW_ask_work_view(&work_view) );  
        // Cycle all modeling views and unset explosions  
        n_views = ask_all_modeling_views(part_tag, &views);  
        for (ii = 0; ii < n_views; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(views[ii], view_name));  
            //uc6464("", "", view_name);  
            ECHO("Unsetting Explosion from view %s\n", view_name);  
            UF_CALL( UF_ASSEM_set_view_explosion ( views[ii], NULL_TAG));  
        }  
        if( n_views ) UF_free(views);  
        // now delete all explosions  
        UF_CALL( UF_ASSEM_ask_explosions(part_tag, &n_explosions, &explosion_tags));  
        for( ii=0; ii<n_explosions; ii++)  
        {  
            ECHO("Deleting Explosion %d\n", explosion_tags[ii]);  
            UF_CALL( UF_ASSEM_delete_explosion(explosion_tags[ii]) );  
        }  
        // regenerate the work view  
        UF_CALL( UF_DISP_regenerate_view( work_view ) );  
        /* Terminate the API environment 里海译:终止API环境 */  
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
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发应用程序，主要用于删除当前显示的部件中的所有爆炸视图。以下是代码的主要功能：
>
> 1. 初始化和终止API环境。
> 2. 获取当前显示的部件。
> 3. 备份当前的工作视图。
> 4. 循环所有建模视图，并取消设置爆炸视图。
> 5. 删除当前部件中的所有爆炸视图。
> 6. 重新生成工作视图。
> 7. 提供卸载处理函数，当应用程序被卸载时调用。
>
> 该应用程序的主要逻辑是通过循环所有建模视图，取消设置爆炸视图，然后删除当前部件中的所有爆炸视图，最后重新生成工作视图，从而实现删除所有爆炸视图的目的。
>
> 代码中使用了NX提供的UF库函数，通过调用这些函数实现了与NX系统的交互。整体来说，该应用程序实现了删除爆炸视图的功能，可以帮助用户快速清理当前部件的爆炸视图设置。
>
