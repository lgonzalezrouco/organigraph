function loadData() {
    d3.select("#graph-svg").html(""); // Clear previous graph
    document.getElementById("error-message").style.display = "none"; // Hide previous error message

    let jsonData = null;
    const jsonInput = document.getElementById("json-input").value.trim();
    const fileInput = document.getElementById("file-input");

    if (jsonInput !== "") {
        try {
            jsonData = JSON.parse(jsonInput);
            visualizeData(jsonData);
        } catch (error) {
            showError("Invalid JSON format!");
            showNoDataMessage(true);
        }
    } else if (fileInput.files.length > 0) {
        const file = fileInput.files[0];
        const reader = new FileReader();
        reader.onload = function (event) {
            try {
                jsonData = JSON.parse(event.target.result);
                visualizeData(jsonData);
            } catch (error) {
                showError("Invalid JSON file!");
                showNoDataMessage(true);
            }
        };
        reader.readAsText(file);
    } else {
        showError("Please paste JSON data or upload a JSON file.");
        showNoDataMessage(true);
    }
}

function visualizeData(data) {
    if (!data || !data.nodes || !data.links) {
        showNoDataMessage(true);
        return;
    }

    const graphContainer = d3.select("#graph-svg");
    const width = graphContainer.node().getBoundingClientRect().width;
    const height = 800;

    graphContainer.html(""); // Clear previous graph

    const svg = graphContainer
        .attr("width", width)
        .attr("height", height)
        .append("g");

    const zoom = d3
        .zoom()
        .scaleExtent([0.1, 10])
        .on("zoom", (event) => {
            containerGroup.attr("transform", event.transform);
        });

    graphContainer.call(zoom);

    const containerGroup = svg.append("g");

    // Define the arrow marker
    containerGroup
        .append("defs")
        .append("marker")
        .attr("id", "arrow")
        .attr("viewBox", "0 -10 20 20")
        .attr("refX", 25) // Adjusted for better arrow placement
        .attr("refY", 0)
        .attr("markerWidth", 15)
        .attr("markerHeight", 15)
        .attr("orient", "auto")
        .append("path")
        .attr("d", "M0,-10L20,0L0,10")
        .attr("fill", "#555");

    const simulation = d3
        .forceSimulation(data.nodes)
        .force(
            "link",
            d3
                .forceLink(data.links)
                .id((d) => d.id)
                .distance(200)
        )
        .force("charge", d3.forceManyBody().strength(-500))
        .force("center", d3.forceCenter(width / 2, height / 2))
        .force(
            "collision",
            d3
                .forceCollide()
                .radius((d) => Math.max(100, (d.name || "").length * 10) / 2)
        );

    const link = containerGroup
        .append("g")
        .attr("class", "links")
        .selectAll("path")
        .data(data.links)
        .enter()
        .append("path")
        .attr("class", "link")
        .attr("marker-end", "url(#arrow)"); // Reference the arrow marker here

    const node = containerGroup
        .append("g")
        .attr("class", "nodes")
        .selectAll("g")
        .data(data.nodes)
        .enter()
        .append("g")
        .attr("class", "node")
        .call(
            d3
                .drag()
                .on("start", dragStarted)
                .on("drag", dragged)
                .on("end", dragEnded)
        );

    node
        .append("rect")
        .attr("width", (d) => {
            const numChildren = d.children ? d.children.length : 0;
            return Math.max(100, (d.id || "").length * 10 + numChildren * 5);
        })
        .attr("height", (d) => {
            const numProperties = Object.keys(d).filter(
                (key) => !["id", "x", "y", "vx", "vy"].includes(key)
            ).length;
            const numChildren = d.children ? d.children.length : 0;
            return 20 + 15 * numProperties + numChildren * 5; // Adjust height based on the number of properties and children
        })
        .attr("x", (d) => {
            const numChildren = d.children ? d.children.length : 0;
            return -Math.max(100, (d.id || "").length * 10 + numChildren * 5) / 2;
        })
        .attr("y", (d) => {
            const numProperties = Object.keys(d).filter(
                (key) => !["id", "x", "y", "vx", "vy"].includes(key)
            ).length;
            const numChildren = d.children ? d.children.length : 0;
            return -10 - (15 * (numProperties - 1) + numChildren * 5) / 2;
        })
        .attr("rx", 10)
        .attr("ry", 10)
        .style("fill", "#369")
        .style("stroke", "#666")
        .style("stroke-width", "2px")
        .style("cursor", "pointer");

    node
        .append("text")
        .attr("dy", -15)
        .attr("text-anchor", "middle")
        .style("font-size", "14px")
        .style("fill", "#fff")
        .text(""); // Remove the text content assignment

    node
        .append("text")
        .attr("dy", -15) // Adjust as needed
        .attr("text-anchor", "middle")
        .style("font-size", "18px") // Adjust font size here
        .style("fill", "#fff")
        .text((d) => d.id); // Display the 'id' field of the node

    node
        .selectAll("text.prop")
        .data((d) => {
            const properties = Object.keys(d)
                .filter((key) => !["id", "x", "y", "vx", "vy", "index"].includes(key))
                .map((key) => ({key, value: d[key]}));
            return properties.map((prop, i) => ({...prop, yOffset: i * 15}));
        })
        .enter()
        .append("text")
        .attr("class", "prop")
        .attr("dy", (d) => `${d.yOffset}px`)
        .attr("text-anchor", "middle")
        .style("font-size", "10px")
        .style("fill", "#fff")
        .text(
            (d) => `${d.key.charAt(0).toUpperCase() + d.key.slice(1)}: ${d.value}`
        );

    simulation.on("tick", () => {
        link.attr(
            "d",
            (d) => `
      M${d.source.x},${d.source.y}
      L${d.target.x},${d.target.y}
    `
        );

        node.attr("transform", (d) => `translate(${d.x},${d.y})`);
    });

    function dragStarted(event, d) {
        if (!event.active) simulation.alphaTarget(0.3).restart();
        d.fx = d.x;
        d.fy = d.y;
    }

    function dragged(event, d) {
        d.fx = event.x;
        d.fy = event.y;
    }

    function dragEnded(event, d) {
        if (!event.active) simulation.alphaTarget(0);
        d.fx = null;
        d.fy = null;
    }

    showNoDataMessage(false);
}

function showNoDataMessage(show) {
    const messageDiv = document.getElementById("no-data-message");
    if (show) {
        messageDiv.style.display = "block";
    } else {
        messageDiv.style.display = "none";
    }
}

function showError(message) {
    const errorMessageDiv = document.getElementById("error-message");
    errorMessageDiv.innerText = message;
    errorMessageDiv.style.display = "block";
}
