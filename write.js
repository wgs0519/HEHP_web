function toggleContent(id) {
    var content = document.getElementById(id);
    if (content.style.display === "none") {
        content.style.display = "block";
    } else {
        content.style.display = "none";
    }
}

function addPost() {
    var postContent = document.getElementById("postContent").value;
    if (postContent === "") {
        alert("글 내용을 입력하세요.");
        return;
    }
    var postList = document.getElementById("postList");
    var newPost = document.createElement("div");
    newPost.className = "post";

    var postText = document.createElement("p");
    postText.innerText = postContent;

    var commentList = document.createElement("div");
    commentList.className = "commentList";

    var commentInput = document.createElement("input");
    commentInput.type = "text";
    commentInput.placeholder = "댓글을 입력하세요";

    var commentButton = document.createElement("button");
    commentButton.innerText = "댓글 달기";
    commentButton.onclick = function() {
        var commentContent = commentInput.value;
        if (commentContent === "") {
            alert("댓글 내용을 입력하세요.");
            return;
        }
        var newComment = document.createElement("p");
        newComment.innerText = commentContent;
        commentList.appendChild(newComment);
        commentInput.value = "";
    };

    newPost.appendChild(postText);
    newPost.appendChild(commentList);
    newPost.appendChild(commentInput);
    newPost.appendChild(commentButton);
    postList.appendChild(newPost);

    document.getElementById("postContent").value = "";
}
