game.module(
    'game.main'
)
.body(function() {

game.addAsset('cursor.png');
game.addAsset('cursor2.png');
 game.addAsset('hole.png');
 
	game.createClass('Box', 'Sprite', {
	    interactive: true,
	
	    init: function(x, y) {
			//call parent constructor function
	        this._super('hole.png', x, y);
	        game.scene.stage.addChild(this);

	        //text
            this.text = new game.PIXI.Text("hit text\n or box", { font: '30px Arial' });
            this.text.position = {x: 10, y:10};
            this.text.interactive=true;
            this.text.click = function() {
	        	console.log("text on box clicked");
	    	};
			//Text is added as a child to the boxclass
            this.addChild(this.text);
	    },

	    click: function() {
	        console.log("box itself clicked");
	    }
	});

game.createScene('Main', {
    backgroundColor: "#AAAAFF",
    init: function() {

        	this.box = new game.Box(200,150);
			
        this.cursor = new game.Sprite("cursor.png");
        this.cursor.width = 32;
        this.cursor.height = 32;
        this.cursor.addTo(this.stage);
		
    },
    update: function() {
        this.cursor.x = game.input.mouse.x-14;
        this.cursor.y = game.input.mouse.y;
    },
	mousedown: function(){
		game.stage.container.removeChild(this.cursor);
        this.cursor = new game.Sprite("cursor2.png");
        this.cursor.width = 32;
        this.cursor.height = 32;
		
        this.cursor.addTo(this.stage);
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

