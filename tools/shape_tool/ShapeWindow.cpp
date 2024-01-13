#include '../../../../lib/ui3/elements/TileShapeGraphic.cpp'

const array<int> SHAPE_MAP = {
	 8, 16,  0,  0,
	 7, 15,  0,  0,
	11,  3, 20, 17,
	12,  4, 19, 18,
	10,  9,  1,  2,
	 6,  5, 13, 14
};

class ShapeWindow : Window
{
	
	private Container@ shape_container;
	private Button@ custom_button;
	
	private int _tile_shape = 0;
	private int _selection_index = 2;
	
	private bool _selecting_tiles = false;
	private bool _using_custom_shape = false;
	
	private TileShapes@ _selected_shape;
	
	int tile_shape
	{
		get const { return _tile_shape; }
		set
		{
			if(value < 0 || value > 20)
				return;
			int index = SHAPE_MAP.find(value);
			cast<Button>(shape_container.get_child(index)).selected = true;
		}
	}
	
	bool selecting_tiles { get const { return _selecting_tiles; } }
	bool using_custom_shape { get const { return _using_custom_shape; } }
	
	ShapeWindow(UI@ ui)
	{
		super(ui, 'Shape', false);
		
		@layout = FlowLayout(ui, FlowDirection::Row);
		
		@shape_container = Container(ui);
		
		GridLayout@ grid = GridLayout(ui, 4);
		grid.row_spacing = 0;
		grid.column_spacing = 0;
		@shape_container.layout = grid;
		
		ButtonGroup@ button_group = ButtonGroup(ui, false);
		button_group.select.on(EventCallback(on_button_select));
		
		for(int i = 0; i < 24; i++)
		{
			int tile_shape = SHAPE_MAP[i];
			TileShapeGraphic@ t = TileShapeGraphic(ui, tile_shape);
			
			Button@ b = Button(ui, t);
			b.name = '' + i;
			b.width = 48;
			b.height = 48;
			b.selectable = true;
			
			if(i == 2) 
				b.selected = true;
			
			button_group.add(b);
			shape_container.add_child(b);
		}
		
		shape_container.fit_to_contents();
		
		Container@ custom_shape_container = Container(ui);
		@custom_shape_container.layout = FlowLayout(ui, FlowDirection::Row);
		
		Label@ custom_label = Label(ui, 'Clone');
		custom_label.width = 2 * 48 - 2 * ui.style.spacing;
		custom_label.height = 48;
		custom_label.align_h = GraphicAlign::Centre;
		custom_label.align_v = GraphicAlign::Middle;
		
		Button@ select_button = Button(ui, 'X');
		select_button.name = 'select';
		select_button.width = 48;
		select_button.height = 48;
		select_button.selectable = true;
		@select_button.tooltip = PopupOptions(ui, 'Select a region', false, PopupPosition::Below);
		
		@custom_button = Button(ui, 'O');
		custom_button.name = 'custom';
		custom_button.width = 48;
		custom_button.height = 48;
		custom_button.selectable = true;
		custom_button.disabled = true;
		@custom_button.tooltip = PopupOptions(ui, 'Paste tiles', false, PopupPosition::Below);
		
		button_group.add(select_button);
		button_group.add(custom_button);
		
		custom_shape_container.add_child(custom_label);
		custom_shape_container.add_child(select_button);
		custom_shape_container.add_child(custom_button);
		
		custom_shape_container.width = shape_container.width;
		custom_shape_container.fit_to_contents(false);
		
		add_child(shape_container);
		add_child(custom_shape_container);
		
		fit_to_contents(true);
	}
	
	void move_selection(int dx, int dy)
	{
		if(_selecting_tiles || _using_custom_shape)
			return;
		
		const int x = (_selection_index % 4) + dx;
		const int y = (_selection_index / 4) + dy;
		if(x < 0 || x > 3 || y < 0 || y > 5)
			return;
		
		const int index = 4 * y + x;
		cast<Button>(shape_container.get_child(index)).selected = true;
	}
	
	void finished_selection()
	{
		custom_button.disabled = false;
		custom_button.selected = true;
	}
	
	// ///////////////////////////////////////////
	// Events
	// ///////////////////////////////////////////
	
	private void on_button_select(EventInfo@ event)
	{
		if(@event.target == null)
			return;
		
		const string name = event.target.name;
		if(name == 'select')
		{
			_selecting_tiles = true;
			_using_custom_shape = false;
		}
		else if(name == 'custom')
		{
			_selecting_tiles = false;
			_using_custom_shape = true;
		}
		else
		{
			_selecting_tiles = false;
			_using_custom_shape = false;
			_selection_index = parseInt(event.target.name);
			_tile_shape = SHAPE_MAP[_selection_index];
		}
	}
	
}
