### 【2759】set edge density of selected cae faces edges 设置所选CAE面边的边密度

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
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
    /* qq3123197280 */  
    static int mask_for_cae_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_caegeom_type, UF_caegeom_face_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cae_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a CAE face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_cae_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii, n;  
        tag_t  
            *cae_edges,  
            cae_face;  
        UF_SF_edge_density_data_t  
            edge_density_data = { UF_SF_EDGE_DENSITY_SIZE, 1.0, 0 };  
        while ((cae_face = select_a_cae_face("Set Edge Density")) != NULL_TAG)  
        {  
            UF_CALL(UF_SF_face_ask_edges(cae_face, &n, &cae_edges));  
            for (ii = 0; ii < n; ii++)  
            {  
        /*  This fails with an Internal error - See PR 2148259 (fixed in NX7.5.1)*/  
                UF_CALL(UF_SF_set_edge_density(cae_edges[ii], edge_density_data));  
            }  
            if (n > 0) UF_free(cae_edges);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发用户函数(ufusr)。其主要功能是设置CAE网格模型的边密度。
>
> 代码主要包含以下部分：
>
> 1. 错误处理宏：定义了一个宏UF_CALL，用于调用NX API函数，并捕获错误代码。如果函数返回错误，会打印错误信息。
> 2. 选择CAE面：定义了一个函数select_a_cae_face，用于选择一个CAE面。这个函数会弹出一个对话框，提示用户选择一个面，并返回该面的tag。
> 3. 设置边密度：定义了一个函数do_it，用于遍历选择的面上的所有边，并设置每条边的密度。使用了UF_SF_set_edge_density函数来设置边的密度。
> 4. ufusr函数：这是NX二次开发的标准入口函数。首先初始化NX API，然后调用do_it函数执行主逻辑，最后终止NX API。
> 5. 卸载函数：定义了一个卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 该代码通过选择一个CAE面，然后设置该面上所有边的密度，实现了对CAE网格模型边密度的设置。
>
