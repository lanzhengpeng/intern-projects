### 【1648】move all views by specified values 移动所有视图到指定位置

#### 代码

```cpp
    /*HEAD MOVE_ALL_VIEWS_BY_SPECIFIED_VALUES CCC UFUN */  
    /* Moves selected drawing view by specified values 里海译:移动选定绘图视图，按指定值。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }  
    static logical is_drawing_member_view(tag_t view)  
    {  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_VIEW_type_t   
            type;  
        UF_VIEW_subtype_t   
            subtype;  
        UF_CALL(UF_OBJ_ask_name(view, view_name));  
        UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
        if( type == UF_VIEW_DRAWING_MEMBER_TYPE) return(TRUE);  
        else return(FALSE);  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        char   
            view_name[MAX_ENTITY_NAME_SIZE+1],  
            msg[100];  
        double   
            ref_pt[2],  
            values[2],  
            view_bounds[4];  
        tag_t  
            part_tag = UF_PART_ask_display_part(),  
            view_tag = NULL_TAG;  
        if (prompt_for_two_numbers("Enter movement values", "DX", "DY",  
                &values[0], &values[1]))  
        {  
            UF_CALL( UF_UI_open_listing_window());  
            sprintf(msg, "Specified Values: dx:%f, dy:%f\n",values[0], values[1]);  
            UF_UI_write_listing_window(msg);  
            while( (view_tag=ask_next_view(part_tag, view_tag)) != NULL_TAG )  
            {  
                if( is_drawing_member_view(view_tag) )   
                {  
                    UF_CALL(UF_OBJ_ask_name(view_tag, view_name));  
                    sprintf(msg, "\nView %d - %s\n", view_tag, view_name);  
                    UF_UI_write_listing_window(msg);  
                    UF_CALL(uc6485(view_name,view_bounds));  
                    UF_CALL( UF_DRAW_ask_drawing_ref_pt( view_tag, ref_pt) );  
                    sprintf(msg, "Left bound is %f\n",view_bounds[0]);  
                    UF_UI_write_listing_window(msg);  
                    sprintf(msg, "Bottom bound is %f\n",view_bounds[1]);  
                    UF_UI_write_listing_window(msg);  
                    sprintf(msg, "Right bound is %f\n",view_bounds[2]);  
                    UF_UI_write_listing_window(msg);  
                    sprintf(msg, "Top bound is %f\n",view_bounds[3]);  
                    UF_UI_write_listing_window(msg);  
                    sprintf(msg, "Reference Point X: %f, Y: %f\n", ref_pt[0], ref_pt[1]);  
                    UF_UI_write_listing_window(msg);  
                    ref_pt[0] = ref_pt[0] + values[0];  
                    ref_pt[1] = ref_pt[1] + values[1];  
                    sprintf(msg, "New Reference Point X: %f, Y: %f\n", ref_pt[0], ref_pt[1]);  
                    UF_UI_write_listing_window(msg);  
                    if( ask_yes_or_no( "Yes or No", "Move View?") )  
                        UF_CALL( UF_DRAW_set_drawing_ref_pt( NULL_TAG, view_tag, ref_pt) );  
                }  
            }  
        }  
    }  
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

> 这段代码是一个NX的二次开发程序，其主要功能是移动选定的绘图视图。代码的主要流程如下：
>
> 1. 包含了必要的NX开发头文件。
> 2. 定义了错误报告函数report_error，用于在调用NX API失败时输出错误信息。
> 3. 定义了prompt_for_two_numbers函数，用于提示用户输入两个数值，并返回用户输入的值。
> 4. 定义了ask_next_view函数，用于遍历零件中的所有视图，并返回下一个视图的tag。
> 5. 定义了is_drawing_member_view函数，用于判断视图是否为绘图成员视图。
> 6. 定义了ask_yes_or_no函数，用于显示Yes/No对话框，并返回用户的选择结果。
> 7. 定义了do_it函数，该函数是程序的主要逻辑：提示用户输入移动的X和Y值。打开日志窗口，遍历零件中的所有视图。对于每个绘图成员视图，获取视图名称、边界和参考点。计算新的参考点位置。提示用户是否移动视图，如果用户选择Yes，则更新视图的参考点，从而实现视图的移动。
> 8. 提示用户输入移动的X和Y值。
> 9. 打开日志窗口，遍历零件中的所有视图。
> 10. 对于每个绘图成员视图，获取视图名称、边界和参考点。
> 11. 计算新的参考点位置。
> 12. 提示用户是否移动视图，如果用户选择Yes，则更新视图的参考点，从而实现视图的移动。
> 13. 定义了ufusr函数，是程序的入口函数。在初始化NX环境后，调用do_it函数，然后终止NX环境。
> 14. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序卸载时不需要等待。
>
> 综上所述，这段代码实现了一个简单的NX二次开发程序，通过用户输入的数值，移动选定的绘图视图。
>
