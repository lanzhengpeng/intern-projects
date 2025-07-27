### 【2654】select drawing member view 选择绘图成员视图

#### 代码

```cpp
    static tag_t select_drawing_member_view(char *prompt)  
    {  
        int  
            orig_cv,  
            resp;  
        tag_t  
            view_tag;  
        double  
            pos[3];  
        UF_CALL(UF_UI_ask_cursor_view(&orig_cv));  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_specify_screen_position(prompt, NULL, NULL, pos,  
            &view_tag, &resp));  
        UF_CALL(UF_UI_set_cursor_view(orig_cv));  
        if (resp == UF_UI_PICK_RESPONSE)  
            return view_tag;  
        else  
            return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于选择绘图成员视图的函数。其主要功能是提示用户选择一个视图，并返回该视图的tag。
>
> 具体步骤如下：
>
> 1. 获取当前视图的tag并保存。
> 2. 设置当前视图为根视图。
> 3. 使用UI库的函数提示用户选择一个视图，并获取用户选择的视图tag和响应码。
> 4. 恢复到原始视图。
> 5. 如果用户响应为选择响应，则返回选择的视图tag；否则返回空tag。
>
> 通过这个函数，用户可以方便地从NX的多个视图窗口中选择一个视图，以进行后续操作，如绘图、测量等。函数通过UI交互，提高了易用性。
>
