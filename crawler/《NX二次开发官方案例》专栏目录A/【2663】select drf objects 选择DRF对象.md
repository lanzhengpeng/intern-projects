### 【2663】select drf objects 选择DRF对象

#### 代码

```cpp
    static int select_drf_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_class_dialog( "Select objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于在NX中实现选择物体的功能。
>
> 主要功能包括：
>
> 1. 使用UF_UI_set_cursor_view函数隐藏鼠标光标，以避免选择时鼠标遮挡物体。
> 2. 调用UF_UI_select_with_class_dialog函数弹出一个选择对话框，提示用户选择物体，对话框标题为"Select objects"，提示文字为传入的prompt参数，选择范围限定在工作部件内。
> 3. 将选择结果存储在传入的objects参数指向的数组中，并返回选择的物体数量cnt。
> 4. 遍历选择结果，使用UF_DISP_set_highlight函数关闭物体的高亮显示。
> 5. 最后恢复鼠标光标显示。
>
> 总体而言，这段代码通过UI函数实现了用户选择物体的交互功能，并返回了选择结果。
>
