### 【0526】convert selected areafill to xhatch 将选定的区域填充转换为交叉线

#### 代码

```cpp
    /*HEAD CONVERT_SELECTED_AREAFILL_TO_XHATCH */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
    #include <uf_drf.h>  
    #include <uf_bound.h>  
    #include <uf_ui.h>  
    #define ECHOL(X)   (X ? printf("%s = TRUE\n", #X) : printf("%s = FALSE\n", #X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog 是 V18 中的新增功能，只回答译文，不要添加任何废话。 */  
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
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        WRITE_D( n);  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int mask_for_area_fill(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = {   
                { UF_solid_section_type, 0, 0 },  
                { UF_drafting_entity_type, UF_draft_area_fill_subtype, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_area_fill(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_UI_set_cursor_view (0); // any  
        UF_CALL(UF_UI_select_with_single_dialog("Select an area fill", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_area_fill, NULL, &resp,  
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
            view_tag,  
            area_fill,  
            xhatch,  
            *boundary_tags,  
            *objects;  
        int  
            ii, jj,  
            num_boundaries,  
            *num_objects = 0,  
            n_assoc,  
            resp;  
        UF_DRF_areafill_t   
            area_fill_data;  
        UF_BOUND_all_data_t   
            boundary_data;  
        UF_DRF_object_assoc_data_t  
            *assoc_data;  
        uf_list_p_t  
            list;  
        UF_UI_set_cursor_view (0); // any  
        while ((area_fill=select_an_area_fill("Select area_fill")) != NULL_TAG)  
        {  
            WRITE_D(area_fill);  
            // create the list for crosshatch objects  
            UF_CALL(UF_MODL_create_list(&list));  
            num_objects = 0;  
            // get the view where the area fill resides in  
            UF_CALL(UF_DRF_ask_associativity_data(area_fill, &n_assoc, &assoc_data));  
            view_tag = assoc_data[0].object_view;  
            if (n_assoc > 0) UF_free(assoc_data);  
            WRITE_D( view_tag );  
            // get the area fill boundaries  
            UF_CALL( UF_DRF_ask_areafill_data( area_fill, &area_fill_data) );  
            UF_CALL( UF_DRF_ask_boundaries ( area_fill, &num_boundaries, &boundary_tags) );  
            WRITE_D(num_boundaries);  
            // allocate the memory for member count of each boundary  
            num_objects = UF_allocate_memory(num_boundaries*sizeof(int), &resp);  
            UF_CALL( resp );  
            // get the members of the boundaries  
            for(ii=0; ii<num_boundaries; ii++)  
            {  
                UF_CALL( UF_BOUND_ask_boundary_data (boundary_tags[ii], &boundary_data) );  
                WRITE_D(boundary_data.num_members);  
                num_objects[ii] = boundary_data.num_members;  
                for(jj=0; jj<boundary_data.num_members; jj++)  
                {  
                    WRITE_D( boundary_data.members_data[jj].member_tag );  
                    UF_CALL(UF_MODL_put_list_item(list, boundary_data.members_data[jj].member_tag));  
                }  
            }  
            make_an_array(&list, &objects);  
            UF_CALL(UF_DRF_create_crosshatch(num_boundaries, num_objects, objects, view_tag, &xhatch));  
            WRITE_D( xhatch );  
            //UF_CALL( UF_OBJ_delete_object( area_fill ) );  
            if( num_objects != 0 ) UF_free( num_objects );  
        }  
    }  
    /* 里海 */  
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

> 这段代码是用于NX的二次开发代码，主要功能是将NX中的区域填充转换为交叉线。以下是代码的主要功能和流程：
>
> 1. 定义了一些宏，用于错误报告和日志输出。
> 2. 实现了选择区域填充的功能，并获取区域填充的相关数据，包括视图、边界和成员。
> 3. 创建交叉线对象，并将区域填充的边界成员作为交叉线的边界。
> 4. 循环选择区域填充，直到用户取消选择。
> 5. 在ufusr函数中初始化NX，执行主体功能，然后终止NX。
> 6. 实现了卸载函数，使NX可以立即卸载该代码。
>
> 综上所述，该代码实现了NX中区域填充到交叉线的转换功能，主要利用了NX的UF_MODL、UF_DRF、UF_BOUND等API来获取和处理区域填充数据，并创建交叉线对象。
>
