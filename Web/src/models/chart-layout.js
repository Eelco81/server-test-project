
export default function(id) {
    
    return {
        title: id,
        xaxis: {
            showgrid: true,
            showline: true,
            zeroline: false,
            tickfont: {
            }
        },
        yaxis: {
            showgrid: true,
            showline: true,
            zeroline: true,
            tickfont: {
            }
        },
        dragmode: false,
        hovermode: false,
        showlegend: true,
        legend: {
            x: 0,
            y: -0.5
        },
        margin: {
            l: 30,
            r: 30,
            t: 30,
            b: 30
        }
    };

}