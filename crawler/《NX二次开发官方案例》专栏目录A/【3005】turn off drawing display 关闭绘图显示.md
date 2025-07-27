### 【3005】turn off drawing display 关闭绘图显示

#### 代码

```cpp
    static void turn_off_drawing_display()  
    {  
        UF_CALL(UF_DRAW_set_display_state(1));  
    }

```

#### 代码解析

> 根据代码片段，这是一个用于关闭NX绘图显示的NX二次开发代码。
>
> 具体来说：
>
> 1. 这是一个静态函数，可以直接调用。
> 2. 函数名为turn_off_drawing_display，表示其功能是关闭绘图显示。
> 3. 函数内部调用了UF_DRAW_set_display_state函数。这个函数用于设置NX绘图显示状态。
> 4. 参数1表示关闭绘图显示。
> 5. 因此，该函数通过调用UF_DRAW_set_display_state(1)来关闭NX的绘图显示。
> 6. 作者编写这个函数的目的是为了在二次开发中方便地控制NX的绘图显示状态。
>
> 综上所述，这段代码通过调用NX的UF_DRAW_set_display_state函数，实现了一个静态函数turn_off_drawing_display，用于关闭NX的绘图显示。
>
