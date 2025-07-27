### 【3149】report material name of selected body 选择一个实体，获取其材质名称，并打印出来

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_sf.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <stdarg.h>  
    //  
    // First tested against NX7.0.1, with materials assigned  
    // via Tools -> Material Properties  
    //  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            source_body = NULL_TAG,  
            material = NULL_TAG,  
            orig_material = NULL_TAG,  
            orig_part = NULL_TAG,  
            matl_owning_part = NULL_TAG,  
            part = NULL_TAG;  
        char     
            *mat_name,  
            msg[257] = { "" };  
        UF_SF_material_prop_t   
            props;   
        orig_part = UF_PART_ask_display_part();  
        if( NULL_TAG == orig_part ) return;  
        if(( source_body = select_a_body("Select Body:")) !=NULL_TAG )  
        {  
            if( UF_ASSEM_is_occurrence( source_body ))  
            {  
                source_body = UF_ASSEM_ask_prototype_of_occ( source_body );  
            }  
            UF_CALL(UF_SF_init_matl_prop(&props));  
            UF_CALL( UF_SF_locate_material( source_body, &material ));    
            if( NULL_TAG == material )  
            {  
                uc1601( "Selected Body has no Material assigned.", TRUE );   
                return;    
            }         
            UF_CALL( UF_OBJ_ask_owning_part( material, &matl_owning_part ));           
            UF_CALL( UF_SF_ask_material( material, &mat_name, &props ));           
            sprintf( msg, "Material Name: %s\n", mat_name );  
            WRITE( msg );  
            UF_SF_free_matl_prop(&props);  
            UF_free( mat_name );  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的NX头文件，用于调用NX的API函数。
> 2. 定义了ECHO宏，用于输出信息到列表窗口和系统日志。
> 3. 定义了UF_CALL宏，用于调用NX的API函数，并捕获错误。
> 4. 定义了mask_for_bodies函数，用于设置选择对话框的遮罩，只允许选择实体。
> 5. 定义了select_a_body函数，用于弹出一个对话框，让用户选择一个实体，并返回其tag。
> 6. 定义了do_it函数，用于实现主体逻辑：获取当前显示的部件。选择一个实体。检查实体是否有材料，如果没有，则提示并返回。获取实体的材料，并输出材料名称。
> 7. 获取当前显示的部件。
> 8. 选择一个实体。
> 9. 检查实体是否有材料，如果没有，则提示并返回。
> 10. 获取实体的材料，并输出材料名称。
> 11. 定义了ufusr函数，作为程序的入口点，初始化NX环境，调用do_it函数，然后终止NX环境。
> 12. 定义了ufusr_ask_unload函数，用于在卸载用户函数库时立即清理。
>
> 总的来说，这段代码实现了在NX中获取用户选择的实体材料名称的功能。
>
