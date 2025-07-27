### 【2034】report components with faceted models 组件与带面的模型报告

#### 代码

```cpp
    /*HEAD REPORT_COMPONENTS_WITH_FACETED_MODELS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_weld.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHO3F(X)  (printf("%s = %f, %f, %f\n", #X, *X,*(X+1),*(X+2)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = ask_next_of_type(part, type, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        tag_t  
            instance;  
        if ((instance = UF_ASSEM_ask_inst_of_part_occ(component)) != NULL_TAG)  
            UF_CALL(UF_ASSEM_ask_part_name_of_child(instance, partname));  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0,  
            n_fctd_mdls = 0;  
        tag_t  
            comp = NULL_TAG,  
            comp_part = NULL_TAG,  
            *fctd_mdls,  
            part = UF_PART_ask_display_part();  
        char  
            cspec[MAX_FSPEC_SIZE+1],  
            msg[133];  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ(comp);  
            n_fctd_mdls = ask_all_of_type(comp_part, UF_faceted_model_type, &fctd_mdls);  
            if (n_fctd_mdls > 0)  
            {  
                ii++;  
                UF_CALL(UF_DISP_set_highlight(comp, 1));  
                ask_component_partname(comp, cspec);  
                sprintf(msg, "Found %d faceted model(s) in part %s", n_fctd_mdls, cspec);  
                uc1601(msg, TRUE);  
                UF_CALL(UF_DISP_set_highlight(comp, 0));  
                UF_free(fctd_mdls);  
            }  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        }  
        sprintf(msg, "Found %d component(s) having faceted models", ii);  
        uc1601(msg, TRUE);  
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

> 这段代码是用于NX的二次开发，其主要功能是遍历当前打开的零件中的所有组件，并找出包含facet模型的组件。
>
> 主要步骤包括：
>
> 1. 初始化NX环境
> 2. 遍历当前零件中的所有组件
> 3. 对于每个组件，获取其原型零件
> 4. 在原型零件中查找所有facet模型
> 5. 如果找到facet模型，则高亮显示该组件，并输出相关信息
> 6. 继续遍历直到所有组件都被检查
> 7. 结束NX环境
>
> 该代码利用了NX的UF对象操作、零件管理和装配管理等功能来实现遍历组件和查找facet模型。通过二次开发，扩展了NX的功能，实现了对组件的检查和facet模型的识别。
>
