game.module(
    'game.main'
)
.body(function() {

game.addAsset('hand.png');

game.createScene('Main', {
    backgroundColor: "#fff",
    init: function() {
        this.cursor = new game.Sprite("hand.png");
        this.cursor.width = 80;
        this.cursor.height = 80;
        this.cursor.addTo(this.stage);
    },
    update: function() {
        this.cursor.x = game.input.mouse.x;
        this.cursor.y = game.input.mouse.y;
    }
});
    
//__MENU__\\    
game.createScene('Menu', {
    backgroundColor: '#1f1b12',
    
    init: function() {
        text.position.y = 12;
        text.position.x = 12;
		text.addTo(this.stage);
	},
    
    mousedown: function() {
		game.system.setScene('Main');
	}
});

});
