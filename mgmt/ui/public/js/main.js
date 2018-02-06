// dragging
(function(a){function e(h){var k,j=this,l=h.data||{};if(l.elem)j=h.dragTarget=l.elem,h.dragProxy=d.proxy||j,h.cursorOffsetX=l.pageX-l.left,h.cursorOffsetY=l.pageY-l.top,h.offsetX=h.pageX-h.cursorOffsetX,h.offsetY=h.pageY-h.cursorOffsetY;else if(d.dragging||l.which>0&&h.which!=l.which||a(h.target).is(l.not))return;switch(h.type){case"mousedown":return a.extend(l,a(j).offset(),{elem:j,target:h.target,pageX:h.pageX,pageY:h.pageY}),b.add(document,"mousemove mouseup",e,l),i(j,!1),d.dragging=null,!1;case!d.dragging&&"mousemove":if(g(h.pageX-l.pageX)+g(h.pageY-l.pageY)<l.distance)break;h.target=l.target,k=f(h,"dragstart",j),k!==!1&&(d.dragging=j,d.proxy=h.dragProxy=a(k||j)[0]);case"mousemove":if(d.dragging){if(k=f(h,"drag",j),c.drop&&(c.drop.allowed=k!==!1,c.drop.handler(h)),k!==!1)break;h.type="mouseup"}case"mouseup":b.remove(document,"mousemove mouseup",e),d.dragging&&(c.drop&&c.drop.handler(h),f(h,"dragend",j)),i(j,!0),d.dragging=d.proxy=l.elem=!1}return!0}function f(b,c,d){b.type=c;var e=a.event.dispatch.call(d,b);return e===!1?!1:e||b.result}function g(a){return Math.pow(a,2)}function h(){return d.dragging===!1}function i(a,b){a&&(a.unselectable=b?"off":"on",a.onselectstart=function(){return b},a.style&&(a.style.MozUserSelect=b?"":"none"))}a.fn.drag=function(a,b,c){return b&&this.bind("dragstart",a),c&&this.bind("dragend",c),a?this.bind("drag",b?b:a):this.trigger("drag")};var b=a.event,c=b.special,d=c.drag={not:":input",distance:0,which:1,dragging:!1,setup:function(c){c=a.extend({distance:d.distance,which:d.which,not:d.not},c||{}),c.distance=g(c.distance),b.add(this,"mousedown",e,c),this.attachEvent&&this.attachEvent("ondragstart",h)},teardown:function(){b.remove(this,"mousedown",e),this===d.dragging&&(d.dragging=d.proxy=!1),i(this,!0),this.detachEvent&&this.detachEvent("ondragstart",h)}};c.dragstart=c.dragend={setup:function(){},teardown:function(){}}})(jQuery);


$(function(){
	var default_timeline = [
		[0, 3000],
		[19800, 3000],
		[25560, 4991],
		[28800, 5317],
		[32040, 5494],
		[38880, 5659],
		[43200, 5684],
		[47520, 5662],
		[54720, 5492],
		[58320, 5285],
		[61920, 4870],
		[67320, 3000],
		// dummy
		[86400, 3000]
	];

	var id = $("#plot").data("lamp");
	$.getJSON("/api/timeline/" + id, function(timeline) {
		var data;
		if (timeline.length) {
			// mirror midnight point
			timeline.push([24*60*60, timeline[0][1]]);

			data = JSON.parse(JSON.stringify(timeline));
		} else {
			data = JSON.parse(JSON.stringify(default_timeline));
		}

		var plot = $.plot("#plot",
			[
				data
			],
			{
				series: {
					lines: {show: true},
					points: {show: true}
				},
				yaxis: {
					min: 2500,
					max: 6500
				},
				xaxis: {
					min: 0,
					max: 86400,
					ticks: function(axis) {
						var ticks = [];
						for (var i = 0; i <= 24; i += 2) {
							ticks.push(i * 3600);
						}

						return ticks;
					},
					tickFormatter: function(val, axis) {
						var h = Math.trunc(val / 3600);
						var m = Math.trunc((val - 3600 * h) / 60);

						if (h < 10) { h = "0" + h; }
						if (m < 10) { m = "0" + m; }

						return h + ":" + m;
					}
				}
			}
		);

		$("#plot").mousedown(function(e) {e.preventDefault();});
		$("#plot").bind("dblclick", function(e) {
			var offset = plot.offset(),
				canvasX = e.pageX - offset.left,
				canvasY = e.pageY - offset.top;

			var pos = plot.c2p({left: canvasX, top: canvasY});
			var x = pos.x1;
			var y = pos.y1;

			var item = plot.findNearbyItem(canvasX, canvasY, function(s) { return true; });
			if (item) {
				if (item.dataIndex == 0 || item.dataIndex == data.length - 1)
					return;

				data.splice(item.dataIndex, 1);
			} else {
				if (y < 3000 || y > 6000) {
					return;
				}

				if (x < 0 || x > 86400) {
					return;
				}

				if (data.length >= 20) {
					alert("cannot add more points to timeline");
					return;
				}

				for (var i = 0; i < data.length; i++) {
					if (data[i][0] > x)
						break;
				}

				data.splice(i, 0, [x, y]);
			}

			plot.setData([data]);
			plot.draw();
		});

		var drag = -1;
		$("#plot").bind("dragstart", { distance: 10 }, function(e) {
			if (e.which != 1)
				return;

			var offset = plot.offset(),
				canvasX = e.pageX - offset.left,
				canvasY = e.pageY - offset.top;

			var item = plot.findNearbyItem(canvasX, canvasY, function(s) { return true; });
			if (!item)
				return;

			drag = item.dataIndex;
		});

		function clamp(x, min, max) {
			if (x > max)
				x = max;
			else if (x < min)
				x = min;

			return x;
		}

		$("#plot").bind("drag", function(e) {
			var offset = plot.offset(),
				canvasX = e.pageX - offset.left,
				canvasY = e.pageY - offset.top;

			var pos = plot.c2p({left: canvasX, top: canvasY});

			if (drag < 0) {
				return;
			}

			data[drag][1] = clamp(pos.y1, 3000, 6000);

			if (drag == 0 || drag == data.length - 1)
				data[data.length - 1 - drag][1] = data[drag][1];

			if (drag != 0 && drag != data.length - 1)
				data[drag][0] = clamp(pos.x1, data[drag-1][0], data[drag+1][0]);

			plot.setData([data]);
			plot.draw();
		});

		$("#plot").bind("dragend", function(e) {
			drag = -1;
		});

		$("#reset").click(function(e) {
			data = JSON.parse(JSON.stringify(timeline));
			plot.setData([data]);
			plot.draw();
		});

		$("#default").click(function(e) {
			data = JSON.parse(JSON.stringify(default_timeline));
			plot.setData([data]);
			plot.draw();
		});

		$("#clear").click(function(e) {
			data = JSON.parse(JSON.stringify([[0, 3000], [24*60*60, 3000]]));
			plot.setData([data]);
			plot.draw();
		});

		$("#save").click(function(e) {
			post_data = JSON.parse(JSON.stringify(data));

			// remove virtual point
			post_data.splice(post_data.length -1 , 1);

			$.ajax({
				type: "POST",
				url: "/api/timeline/" + id,
				dataType: "text",
				data: JSON.stringify(post_data),
				success: function(rc) {
					if (rc == "ok") {
						timeline = JSON.parse(JSON.stringify(data));
						alert("saved");
					} else {
						alert("error: " + rc);
					}
				},
				error: function(jqXHR, textStatus) {
					alert("failed to connect to server");
				}
			});
		});
	});
});
