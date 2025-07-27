### 【2690】select point near anything 选择任何物体的附近点

#### 代码

```cpp
    static tag_t select_point_near_anything(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要用于在NX中实现选择点功能的函数。具体功能如下：
>
> 1. 调用UF_UI_set_cursor_view函数，设置光标视图为0，表示在模型空间选择。
> 2. 调用UF_UI_select_with_single_dialog函数，弹出一个对话框，提示用户选择点。对话框标题为"Select object"，提示信息为prompt参数，选择范围为当前工作部件，回调函数为NULL。
> 3. 函数返回用户响应码resp和选择的标签object。如果响应码为UF_UI_OBJECT_SELECTED或UF_UI_OBJECT_SELECTED_BY_NAME，表示用户选择了对象。
> 4. 调用UF_DISP_set_highlight函数，取消高亮显示选择的点。
> 5. 如果用户选择了点，返回选择的点标签；否则返回NULL_TAG。
>
> 总的来说，这段代码通过二次开发实现了在NX中选择点的功能，为后续开发提供了便利。
>
