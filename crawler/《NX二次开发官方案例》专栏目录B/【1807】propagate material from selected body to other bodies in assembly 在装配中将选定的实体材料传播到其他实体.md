### 【1807】propagate material from selected body to other bodies in assembly 在装配中将选定的实体材料传播到其他实体

#### 代码

```cpp
    /*HEAD PROPAGATE_MATERIAL_FROM_SELECTED_BODY_TO_OTHER_BODIES_IN_ASSEMBLY CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog是V18版本新增的功能，请只提供翻译，不要添加任何废话。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0;  
        tag_t  
            source_body = NULL_TAG,  
            next_body = NULL_TAG,   
            material = NULL_TAG,  
            orig_material = NULL_TAG,  
            orig_part = NULL_TAG,  
            matl_owning_part = NULL_TAG,  
            part = NULL_TAG;  
        char     
            *mat_name;  
        UF_SF_material_prop_t   
            props;   
        UF_SF_neutral_material_types_t    
            material_type;  
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
            UF_CALL( UF_SF_ask_material_type ( material, &material_type ));  
            orig_material = material;  
            UF_DISP_set_display( UF_DISP_SUPPRESS_DISPLAY );  
            for( inx = 0; inx < UF_PART_ask_num_parts(); inx++ )   
            {  
                next_body = NULL_TAG;  
                part = UF_PART_ask_nth_part( inx );  
                UF_ASSEM_set_work_part ( part );  
                if( part != matl_owning_part )  
                {  
                    UF_CALL( UF_SF_create_material ( mat_name, NULL, material_type,   
                                      &props, UF_SF_MATL_IS_EDITABLE, &material ));  
                }  
                else  
                {  
                    material = orig_material;     
                }  
                while( (next_body=ask_next_body(part, next_body))!=NULL_TAG)  
                {  
                    if( source_body == next_body ) continue;  
                    if( UF_ASSEM_is_occurrence( next_body )) continue;  
                    UF_CALL( UF_SF_link_material(material, next_body));   
                }  
                UF_ASSEM_set_work_part( orig_part );  
            }  
            UF_SF_free_matl_prop(&props);  
            UF_free( mat_name);  
        }  
        UF_DISP_set_display( UF_DISP_UNSUPPRESS_DISPLAY );  
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

> 这段代码是一个NX Open C++应用程序，用于将一个零件中的材料属性复制到装配体中的其他零件。以下是代码的主要功能介绍：
>
> 1. 错误报告函数：定义了一个报告错误函数report_error，用于输出错误信息。
> 2. 选择功能：定义了一个选择函数select_a_body，用于在对话框中让用户选择一个零件体。
> 3. 循环遍历：定义了一个循环遍历函数ask_next_body，用于遍历当前零件中的所有体。
> 4. 复制材料属性：在do_it函数中，首先获取用户选择的源零件体，然后获取该体的材料属性。接着遍历装配体中的所有零件，如果当前零件不是材料所属的原始零件，就创建一个新的材料。然后遍历当前零件中的所有体，如果体不是源零件体，就将新创建的材料关联到该体上。
> 5. 程序入口点：在ufusr函数中，初始化NX Open，调用do_it函数，然后结束NX Open。
> 6. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于立即卸载应用程序。
>
> 通过这个程序，用户可以方便地将一个零件的材料属性应用到整个装配体，大大简化了操作。
>
