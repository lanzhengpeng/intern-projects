### 【2986】transform objects in selected group from point to point 将选中的组中的对象从一点变换到另一点

#### 代码

```cpp
    /*HEAD TRANSFORM_OBJECTS_IN_SELECTED_GROUP_FROM_POINT_TO_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_trns.h>  
    #include <uf_group.h>  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int mask_for_groups(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_group_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_group(char *prompt)  
    {  
        int  
            ii,  
            member_count,  
            resp;  
        double  
            cp[3];  
        tag_t  
            *member_list,  
            group,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a group", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_groups, NULL, &resp,  
            &group, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(group, 0));  
            UF_CALL(UF_GROUP_ask_group_data(group, &member_list, &member_count));  
            for (ii = 0; ii < member_count; ii++)  
               UF_CALL(UF_DISP_set_highlight(member_list[ii], 0));  
            if (member_count > 0) UF_free(member_list);  
            return group;  
        }  
        else return NULL_TAG;  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static void do_it(void)  
    {  
        int  
            off = 2,  
            orig = 0,  
            move = 1,  
            resp = 0,  
            resp2 = 0,  
            resp3 = 0,  
            group_count = 0;  
        tag_t  
            view = NULL_TAG,  
            *members = NULL,  
            group = NULL_TAG;  
        double  
            origin[3] = { 0., 0., 0. },  
            destination[3] = { 0., 0., 0. },  
            delta[3] = { 0., 0., 0. },  
            mx[12];  
        uf_list_p_t  
            object_list;  
        if( UF_PART_ask_num_parts() == 0 ) return;    
        if( ((group=select_a_group("Select Group:")) != NULL_TAG ) &&  
                (!UF_CALL(UF_UI_specify_screen_position("Select Origin:", NULL,   
                    NULL, origin, &view, &resp))) &&  
                resp == UF_UI_PICK_RESPONSE &&  
                (!UF_CALL(UF_UI_specify_screen_position("Select Destination:",   
                    NULL, NULL, destination, &view, &resp2))) &&  
                resp2 == UF_UI_PICK_RESPONSE  
          )  
        {  
            UF_CALL( UF_GROUP_ask_group_data( group, &members, &group_count ));  
            object_list = make_a_list(group_count, members);  
            UF_CALL(UF_MODL_delete_object_parms(object_list));  
            UF_VEC3_sub ( destination, origin, delta );  
            FTN(uf5943)(delta, mx);  
            FTN(uf5947)(mx, members, &group_count, &move, &orig, &off,   
                        NULL, NULL, &resp3);  
            WRITENZ( resp3 );  
            if( group_count > 0 ) UF_free( members );  
            UF_MODL_delete_list(&object_list);  
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

> 这段代码是一个NX Open C++的二次开发示例，主要实现了以下功能：
>
> 1. 错误报告：定义了一个report_error函数，用于在调用UF函数出错时报告错误信息。
> 2. 选择组：定义了一个select_a_group函数，用于通过对话框让用户选择一个组，并返回组的tag。
> 3. 创建对象列表：定义了一个make_a_list函数，用于根据传入的对象数组创建一个UF对象列表。
> 4. 执行变换：定义了一个do_it函数，用于选择组，指定原点位置和目标位置，然后对组内所有对象进行平移变换。其中使用了UF_MODL函数删除对象的变换参数，然后调用底层C函数进行变换。
> 5. 主函数：定义了一个ufusr函数，作为程序的入口点，初始化UF环境，调用do_it函数，然后终止UF环境。
> 6. 卸载请求：定义了一个ufusr_ask_unload函数，返回立即卸载标志，表示当用户关闭程序时应该立即卸载库。
>
> 总体来说，这段代码提供了一个简单的NX Open C++二次开发示例，演示了如何选择组、指定位置，并对组内对象进行平移变换。它具有错误报告、对象选择、对象列表创建和变换等基本功能。
>
