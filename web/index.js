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

  const svg = graphContainer
    .attr("height", height)
    .append("g")
    .attr("transform", "translate(40,40)");

  svg
    .append("defs")
    .append("marker")
    .attr("id", "arrow")
    .attr("viewBox", "0 -5 10 10")
    .attr("refX", 8)
    .attr("refY", 0)
    .attr("markerWidth", 6)
    .attr("markerHeight", 6)
    .attr("orient", "auto")
    .append("path")
    .attr("d", "M0,-5L10,0L0,5")
    .attr("fill", "#555");

  const primaryLinks = data.links.filter((link, i, links) => {
    const targetLinks = links.filter((l) => l.target === link.target);
    return targetLinks[0] === link;
  });

  const root = d3
    .stratify()
    .id((d) => d.id)
    .parentId((d) => {
      const parentLink = primaryLinks.find((link) => link.target === d.id);
      return parentLink ? parentLink.source : null;
    })(data.nodes);

  const treeLayout = d3.tree().size([width - 120, height - 160]);
  treeLayout(root);

  const link = svg
    .selectAll(".link")
    .data(root.links())
    .enter()
    .append("path")
    .attr("class", "link")
    .attr(
      "d",
      d3
        .linkVertical()
        .x((d) => d.x)
        .y((d) => d.y)
    );

  const extraLinks = data.links.filter((link) => {
    const parentLinks = data.links.filter((l) => l.target === link.target);
    return parentLinks.length > 1;
  });

  svg
    .selectAll(".extra-link")
    .data(extraLinks)
    .enter()
    .append("path")
    .attr("class", "link extra-link")
    .attr("d", (d) => {
      const sourceNode = root.descendants().find((n) => n.data.id === d.source);
      const targetNode = root.descendants().find((n) => n.data.id === d.target);
      if (sourceNode && targetNode) {
        return d3
          .linkVertical()
          .x((n) => n.x)
          .y((n) => n.y)({
          source: sourceNode,
          target: targetNode,
        });
      }
    });

  const node = svg
    .selectAll(".node")
    .data(root.descendants())
    .enter()
    .append("g")
    .attr("class", "node")
    .attr("transform", (d) => `translate(${d.x},${d.y})`);

  node
    .append("rect")
    .attr("width", (d) => Math.max(100, d.data.name.length * 10))
    .attr("height", 40)
    .attr("x", (d) => -Math.max(100, d.data.name.length * 10) / 2)
    .attr("y", -20)
    .attr("rx", 10)
    .attr("ry", 10)
    .style("fill", "#369")
    .style("stroke", "#666")
    .style("stroke-width", "2px")
    .style("cursor", "pointer");

  node
    .append("text")
    .attr("dy", ".35em")
    .attr("text-anchor", "middle")
    .style("font-size", "14px")
    .style("fill", "#fff")
    .text((d) => d.data.name);

  node
    .append("text")
    .attr("dy", "1.5em")
    .attr("text-anchor", "middle")
    .style("font-size", "10px")
    .style("fill", "#fff")
    .text((d) => (d.data.age !== undefined ? `Age: ${d.data.age}` : ""));

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
