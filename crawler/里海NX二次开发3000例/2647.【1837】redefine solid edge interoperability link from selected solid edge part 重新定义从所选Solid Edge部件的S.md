### 【1837】redefine solid edge interoperability link from selected solid edge part 重新定义从所选Solid Edge部件的S

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** redefine_se_link_from_selected_par.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 里海译:Include files的翻译为：

包含文件 */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_modl_import_body.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            ext[5] = "*.par";  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        combine_directory_and_wildcard(p_dir, ext, filter);  
        UF_free(p_dir);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static tag_t ask_se_feature(tag_t part)  
    {  
        char  
            msg[100],  
            *type = NULL;  
        tag_t  
            feat = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                type = NULL;  
                UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
                if( type != NULL && strstr(type, ".par", 4) ) return feat;  
            }  
        }  
        return NULL_TAG;  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Unigraphics Startup  
    **      This entry point activates the application at Unigraphics startup */  
    extern DllExport void ufusr( char *param, int *returnCode, int rlen )  
    {  
        tag_t   
            part,  
            se_feat = NULL_TAG;  
        UF_MODL_import_body_feature_edit_option_t   
            edit_option = UF_MODL_import_body_feature_redefine_link;  
        char   
            import_body_filename[MAX_FSPEC_SIZE+1] = { "" };  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 里海译:Failed to initialize */  
            return;  
        }  
        /* TODO: Add your application code here 里海译:翻译 TODO: 在此处添加你的应用程序代码 */  
        part = UF_ASSEM_ask_work_part();  
        se_feat = ask_se_feature(part);  
        if( se_feat == NULL_TAG )  
        {  
            uc1601("No Solid Edge Feature found", 1);  
            return;  
        }  
        if( prompt_for_part_name("New SE PAR",  import_body_filename) )  
        {  
            UF_CALL( UF_MODL_edit_import_body_feature(  
                se_feat,  
                edit_option,  
                import_body_filename ) );  
        }  
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

> 这段代码是一个Unigraphics NX的二次开发示例，其主要功能是重新定义从Solid Edge导入的零件的链接。具体来说，这段代码实现了以下功能：
>
> 1. API环境初始化：使用UF_initialize函数初始化Unigraphics的API环境。
> 2. 获取当前工作部件：通过UF_ASSEM_ask_work_part函数获取当前工作部件。
> 3. 查找Solid Edge特征：通过循环遍历部件中的特征，使用UF_OBJ_cycle_objs_in_part和UF_MODL_ask_feat_type函数，查找类型为".par"的Solid Edge特征。
> 4. 提示用户输入新文件名：通过UF_UI_create_filebox函数提示用户输入新的Solid Edge零件文件名。
> 5. 重新定义特征链接：使用UF_MODL_edit_import_body_feature函数，将找到的Solid Edge特征链接到用户指定的新文件。
> 6. API环境终止：使用UF_terminate函数终止API环境。
> 7. 卸载处理：通过ufusr_ask_unload函数处理程序的卸载。
>
> 整个流程包括初始化、查找Solid Edge特征、提示用户、重新定义特征链接，以及终止和卸载处理。这段代码提供了二次开发中重新定义Solid Edge特征链接的一个典型示例。
>
