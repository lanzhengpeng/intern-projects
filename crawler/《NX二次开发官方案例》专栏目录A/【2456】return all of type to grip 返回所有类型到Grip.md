### 【2456】return all of type to grip 返回所有类型到Grip

#### 代码

```cpp
    /*HEAD RETURN_ALL_OF_TYPE_TO_GRIP CCC UFUN */  
    /* This user function program is called by the following grip program and   
        it passes an array of drafting entities to grip and the grip program  
        changes the color to red.  Grip can do it's own cycling, but this   
        was useful because the user did not want to cycle through subdirectories  
        and components.  
    */  
    /* This is the grip code used:   
    ENTITY/ dents(100)  
    NUMBER/ count  
    GRARGS/ dents, count  
    count = 0  
    XSPAWN/ ufun, 'w:\1nx.dll'  
    IFTHEN/ count < 1  
        MESSG/'None Found'  
    ELSE  
    DO/ A20:, ii, 1, count  
        &COLOR(dents(ii))=&RED    
    A20:  
    ENDIF  
    HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    #define GRIP_ARG_COUNT 2   
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
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static void do_it(void)  
    {     
        tag_t     
            *dents,   
            drents[100],   
            part = UF_PART_ask_display_part();  
        UF_args_t    
            grip_arg_list[GRIP_ARG_COUNT];   
        int       
            n,   
            ii;  
        double  
            count;  
        grip_arg_list[0].type    = UF_TYPE_TAG_T_ARRAY;   
        grip_arg_list[0].length  = 100;   
        grip_arg_list[0].address = drents;  
        grip_arg_list[1].type = UF_TYPE_DOUBLE;   
        grip_arg_list[1].length = 0;   
        grip_arg_list[1].address = &count;   
        /* Update the local arguments with the contents from the GRIP variables 里海译:更新本地参数为GRIP变量内容 */    
        UF_CALL(UF_ask_grip_args (GRIP_ARG_COUNT, grip_arg_list));   
        /* DO STUFF 里海译:翻译DO STUFF为：做事情 */    
        n = ask_all_of_type(part, UF_drafting_entity_type, &dents);  
        count = (double)n;  
        printf("number found %d\n", n);  
        for (ii = 0; ii < n ; ii++ )  
        {  
            drents[ii] = dents[ii];  
        }  
      /* Update the GRIP arguments with the contents from the local variables 里海译:更新GRIP参数，用本地变量的内容代替。 */    
        UF_CALL(UF_set_grip_args (GRIP_ARG_COUNT, grip_arg_list));   
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        /* Terminate User Function 里海译:终止用户函数 */    
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，其主要功能是将NX部件中的所有草绘实体传递给GRIP程序，并让GRIP程序将它们的颜色设置为红色。
>
> 具体步骤包括：
>
> 1. 定义了头文件，包含了必要的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于在函数调用出错时报告错误信息。
> 3. 定义了一个内存分配函数allocate_memory，用于分配内存。
> 4. 定义了一个函数make_an_array，用于将对象列表转换为对象数组。
> 5. 定义了一个函数ask_all_of_type，用于查询部件中所有指定类型的对象，并返回对象数组。
> 6. 定义了一个函数do_it，用于执行主要逻辑：获取当前显示的部件。定义一个参数数组，用于与GRIP程序交换参数。调用UF_ask_grip_args从GRIP获取参数。调用ask_all_of_type获取部件中的所有草绘实体。将实体数组和数量传递给GRIP。调用UF_set_grip_args将参数传递给GRIP。
> 7. 获取当前显示的部件。
> 8. 定义一个参数数组，用于与GRIP程序交换参数。
> 9. 调用UF_ask_grip_args从GRIP获取参数。
> 10. 调用ask_all_of_type获取部件中的所有草绘实体。
> 11. 将实体数组和数量传递给GRIP。
> 12. 调用UF_set_grip_args将参数传递给GRIP。
> 13. 定义了ufusr函数，用于初始化NX并调用do_it函数。
> 14. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 通过调用do_it函数，可以获取当前部件中的所有草绘实体，并通过GRIP参数传递给GRIP程序，由GRIP程序负责将它们的颜色设置为红色。
>
