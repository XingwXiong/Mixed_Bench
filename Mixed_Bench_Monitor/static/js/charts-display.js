var figs = [];
Highcharts.setOptions({
	global: {
		useUTC: false
	},
    //credits: false
    credits: {
        text: '©2018 XingwXiong',
        href: 'https://github.com/XingwXiong/Mixed_Bench'
    }
});
var sys_config = {}
// 判断是否checkbox是否被选择
function checked(ele) {
    // ele 是jquery 对象
    var workload = ele.val();
    if(ele.is(":checked") == false) {
		$("#" + workload + "-report").hide();
	} else {
		$("#" + workload + "-report").show();
		//console.log("#" + workload + "-report");
    }
}
// 显示延迟（含总延迟、服务延迟、排队延迟）的概率分布图（延迟-分位点）
function show_latency_probility(app_name, html_item) {
	figs[app_name]['latency-probility'] = Highcharts.chart(html_item, {
		chart: {
			type: 'line'
		},
		title: {
			text: app_name + '延迟概率分布图'
		},
		xAxis: {
			type: 'linear',
			title: {
				text: '分位点（单位：%）',
			},
			tickInterval: 10,
			tickWidth: 1,
			min: 0,
			max: 100
		},
		yAxis: {
			title: {
				text: '延迟(单位：ms)'
			}
		},
		tooltip: {
			formatter: function () {
				return '<b>' + this.series.name + '</b><br/>' + this.x + '%<br/>' + Highcharts.numberFormat(this.y, 2);
			}
		},
		plotOptions: {
			line: {
				dataLabels: {
					// 开启数据标签
					enabled: true,
                    formatter: function() {
                        return Highcharts.numberFormat(this.y, 2);
                    }
				},
				enabledMouseTracking: false
			}
		},
		series: [{
			name: '总延迟',
		}, {
			name: '排队延迟',
		}, {
			name: '服务延迟',
		}],
	});
}
// 显示延迟（含总延迟、服务延迟、排队延迟）的时序分布图（延迟-时序）
function show_latency_interval(app_name, html_item) {
	var interval = $('#sampling-interval').val();
	if(interval == undefined || interval == NaN || interval <= 1) interval = 1;
    interval = interval * 1000;
	figs[app_name]['latency-interval'] = Highcharts.chart(html_item, {
		chart: {
			type: 'line',
//		        zoomType: 'x'	
        },
		title: {
			text: app_name + '延迟时序分布图'
		},
		xAxis: {
			title: {
				text: '时间（单位）',
			},
			type: 'datetime',
			minRange: interval
		},
		yAxis: {
			title: {
				text: '延迟(单位：ms)'
			}
		},
		tooltip: {
			formatter: function () {
				return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2);
			}
		},
		plotOptions: {
			line: {
				dataLabels: {
					// 开启数据标签
					enabled: false
				},
				enabledMouseTracking: false
			}
		},
		series: [{
			name: 'Mean',
		}, {
			name: 'MIN',
            visible: false
		}, {
			name: 'MAX',
            visible: false
		}, {
			name: '90%',
		}, {
			name: '99%',
		}, {
			name: '99.9%',
		}]
	});
}
// 显示吞吐的时序分布图（吞吐-时序）
function show_through_interval(app_name, html_item) {
	var interval = $('#sampling-interval').val();
	if(interval == undefined || interval == NaN || interval <= 1) interval = 1;
    interval = interval * 1000;
	figs[app_name]['through-interval'] = Highcharts.chart(html_item, {
		chart: {
			type: 'line'
		},
		title: {
			text: app_name + '吞吐时序分布图'
		},
		xAxis: {
			title: {
				text: '时间（单位）',
			},
			type: 'datetime',
			minRange: interval
		},
		yAxis: {
			title: {
				text: '吞吐(单位：qps)'
			},
            labels: {
                formatter: function() {
                    return Highcharts.numberFormat(this.value, 2);
                }
            }
		},
		tooltip: {
			formatter: function () {
				return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2);
			}
		},
		plotOptions: {
			line: {
				dataLabels: {
					// 开启数据标签
					enabled: false
				},
				enabledMouseTracking: false
			}
		},
		series: [{
			name: '吞吐',
			// data: [[0, 7.5], [1, 16.9], [2, 19.5], [3, 24.5], [4, 28.4], [5, 31.5], [6, 35.2], [7, 36.5], [8, 33.3], [9, 28.3], [15, 23.9]]
		}]
	});
}
// 更新请求的进度条
function update_progressbar(html_item, value) {
	$('#' + html_item).attr({
		"aria-valuenow": value
	}).css({
		"width": value + "%"
	}).text(value + "% Finished");
}
/**
 * 显示系统资源利用情况（cpu，mem，disk）
 * 
 */
function show_system_dynamic() {
	var interval = $('#sampling-interval').val();
	if(interval == undefined || interval == NaN || interval <= 1) interval = 1;
    interval = interval * 1000;
	figs['system-cpu-interval'] = Highcharts.chart("system-cpu-interval", {
		chart: {
			type: 'line'
		},
		title: {
			text: 'cpu 使用情况'
		},
		xAxis: {
			title: {
				text: '时间（单位）',
			},
			type: 'datetime',
			minRange: interval
		},
		yAxis: {
			title: {
				text: 'cpu 利用率：(单位：%)'
			},
            labels: {
                formatter: function() {
                    return Highcharts.numberFormat(this.value, 2);
                }
            }
		},
		tooltip: {
			formatter: function () {
				return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2)+"%";
			}
		},
		plotOptions: {
			line: {
				dataLabels: {
					// 开启数据标签
					enabled: false
				},
				enabledMouseTracking: false
			}
		},
		series: [{
			name: 'cpu 利用率',
		}]
	});
    
	figs['system-mem-interval'] = Highcharts.chart("system-mem-interval", {
		chart: {
			type: 'line'
		},
		title: {
			text: 'mem 使用情况'
		},
		xAxis: {
			title: {
				text: '时间（单位）',
			},
			type: 'datetime',
			minRange: interval
		},
		yAxis: {
			title: {
				text: 'mem 利用率：(单位：%)'
			},
            labels: {
                formatter: function() {
                    return Highcharts.numberFormat(this.value, 2);
                }
            }
		},
		tooltip: {
			formatter: function () {
				return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2)+"%";
			}
		},
		plotOptions: {
			line: {
				dataLabels: {
					// 开启数据标签
					enabled: false
				},
				enabledMouseTracking: false
			}
		},
		series: [{
			name: 'mem 利用率',
		}],
    });
	figs['system-net-interval'] = Highcharts.chart("system-net-interval", {
		chart: {
			type: 'line'
		},
		title: {
			text: 'net 使用情况'
		},
		xAxis: {
			title: {
				text: '时间（单位）',
			},
			type: 'datetime',
			minRange: interval
		},
		yAxis: {
			title: {
				text: '网络传输速率：(单位：Mbps)'
			},
            labels: {
                formatter: function() {
                    return Highcharts.numberFormat(this.value, 2);
                }
            }
		},
		tooltip: {
			formatter: function () {
				return '<b>' + this.series.name + '</b><br/>' + Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' + Highcharts.numberFormat(this.y, 2)+"%";
			}
		},
		plotOptions: {
			line: {
				dataLabels: {
					// 开启数据标签
					enabled: false
				},
				enabledMouseTracking: false
			}
		},
		series: [{
			name: 'sent speed',
		},{
            name: 'recv speed',
        }]
    });
}

// 请求结束显示报告

// 静态system数据显示
function show_system_static() {
    var ip_addr = $('#sys-ip-addr').text();
    $.ajax({
        url: '/query_sys_static',
        type: 'GET',
        data: {
            'ip_addr': ip_addr, 
        },
        success: function(data) {
            if(data.cpu_count == undefined) {
                console.log(data);
            } else {
                sys_config['cpu_count'] = data.cpu_count;
                sys_config['mem_total'] = data.mem_total / 1024.0 / 1024.0 / 1024.0;
                sys_config['disk_total'] = data.disk_total / 1024.0 / 1024.0 / 1024.0;
                $('#sys-cpu-cores').html(sys_config['cpu_count'] + " 核");
                $('#sys-mem-total').html(sys_config['mem_total'].toFixed(2) + " GB");
                $('#sys-disk-total').html(sys_config['disk_total'].toFixed(2) + " GB");
            } 
        }
    });
    if(sys_config.cpu_count == undefined) {
        setTimeout("show_system_static", interval * 1000 / 2);
    }
}


// 数据传输
function trasfer() {
	var interval = $('#sampling-interval').val();
	if(interval == undefined || interval == NaN || interval <= 1) interval = 1;
	// 获取选中的checkbox
	$('input[type="checkbox"].chk-workload.chk-tail:checked').each(function(){
		var workload = $(this).val();
		if(figs[workload] == undefined) {
            figs[workload] = [];
            show_latency_probility(workload, workload + '-latency-probility');
            show_latency_interval(workload, workload + '-latency-interval');
            show_through_interval(workload, workload + '-through-interval');
			update_progressbar(workload + "-progress-bar", 0);
		}
        var fig = figs[workload]
        var shift = fig['latency-interval'].series[0].data.length > 10;
		$.ajax({
			url: '/query_app_dynamic',
			type:'GET',
			data: {
				'workload': workload,
				'sampling_size': $('#sampling-size').val()
			}, success: function(data) {
				//console.log(data);
                // 延迟概率分布图
                fig['latency-probility'].series[0].setData(data['fig_lat_prob']['que_lat_prob'], false);
                fig['latency-probility'].series[1].setData(data['fig_lat_prob']['svc_lat_prob'], false);
                fig['latency-probility'].series[2].setData(data['fig_lat_prob']['tot_lat_prob'], true);
                var x = (new Date()).getTime();
				// 延迟时序分布图
                fig['latency-interval'].series[0].addPoint([x,data['fig_lat_intr']['mean_lat']], false, shift);	// Mean Lat
				fig['latency-interval'].series[1].addPoint([x,data['fig_lat_intr']['min_lat']], false, shift);	// 90%
				fig['latency-interval'].series[2].addPoint([x,data['fig_lat_intr']['max_lat']], false, shift);	// 90%
				fig['latency-interval'].series[3].addPoint([x,data['fig_lat_intr']['90_perc']], false, shift);	// 90%
				fig['latency-interval'].series[4].addPoint([x,data['fig_lat_intr']['99_perc']], false, shift);	// 99%
				fig['latency-interval'].series[5].addPoint([x,data['fig_lat_intr']['99.9_perc']], true, shift);	// 99.9%
				// 吞吐时序分布图
				fig['through-interval'].series[0].addPoint([x,data['qps']], true, shift);	// qps

                // 系统资源图
                var mem_percent = 100.0 * data['sys_mem_used'] / (data['sys_mem_used'] + data['sys_mem_free']);
                figs['system-cpu-interval'].series[0].addPoint([x, data['sys_cpu_percent']], true, shift);
                figs['system-mem-interval'].series[0].addPoint([x, mem_percent], true, shift);

                //console.log(workload + ':'+ Highcharts.numberFormat(data['fin_num'] / data['tot_num'] * 100, 0));
			    update_progressbar(workload + "-progress-bar", Highcharts.numberFormat(data['fin_num'] / data['tot_num'] * 100, 0));
			}
		});
	});

	$('input[type="checkbox"].chk-system:checked').each(function(){
        var shift = figs['system-cpu-interval'].series[0].data.length > 10;
        var chk = $(this);
		$.ajax({
			url: '/query_sys_dynamic',
			type:'GET',
			data: {
				'sampling_size': $('#sampling-size').val()
			}, success: function(data) {
				if(data.sys_mem_used == undefined) {
                    console.log(data);
                    chk.attr('disabled', true);
                    chk.attr('checked', false);
                    checked(chk);
                } else {
                    chk.attr('disabled', "false");
                    // 系统资源图
                    var x = (new Date()).getTime();
                    var mem_percent = 100.0 * data['sys_mem_used'] / (data['sys_mem_used'] + data['sys_mem_free']);
                    figs['system-cpu-interval'].series[0].addPoint([x, data['sys_cpu_percent']], true, shift);
                    figs['system-mem-interval'].series[0].addPoint([x, mem_percent], true, shift);
                    var net_sent = Math.max(0.01, data['sys_net_sent']);
                    var net_recv = Math.max(0.01, data['sys_net_resv']);
                    figs['system-net-interval'].series[0].addPoint([x, net_sent], false, shift);
                    figs['system-net-interval'].series[1].addPoint([x, net_recv], true, shift);
                }
			}
		});
	});
    setTimeout("trasfer()", interval * 1000);
}


$(function() {
	interval = $('#sampling-interval').val();
	setTimeout("trasfer()", interval * 1000);
	//$('input[name="workload-type"]').click(function(){
    $('input[type="checkbox"]').each(function() {
        checked($(this));
    }).click(function() {
        checked($(this));
    });
    show_system_static(); 
    show_system_dynamic();
});
