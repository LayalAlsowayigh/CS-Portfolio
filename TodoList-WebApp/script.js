const input = document.getElementById('todo-input');
const addBtn = document.getElementById('add-btn');
const list = document.getElementById('todo-list');

const STORAGE_KEY = 'todo-items-v1';

function loadTodos() {
  try {
    return JSON.parse(localStorage.getItem(STORAGE_KEY)) || [];
  } catch (e) {
    return [];
  }
}

function saveTodos(todos) {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(todos));
}

function render() {
  const todos = loadTodos();
  list.innerHTML = '';
  todos.forEach((t, index) => {
    const li = document.createElement('li');

    const left = document.createElement('div');
    left.className = 'left';

    const cb = document.createElement('input');
    cb.type = 'checkbox';
    cb.checked = t.completed;
    cb.addEventListener('change', () => {
      const updated = loadTodos();
      updated[index].completed = cb.checked;
      saveTodos(updated);
      render();
    });

    const span = document.createElement('span');
    span.textContent = t.title;
    span.className = 'title' + (t.completed ? ' completed' : '');

    left.appendChild(cb);
    left.appendChild(span);

    const del = document.createElement('button');
    del.textContent = 'Delete';
    del.className = 'delete-btn';
    del.addEventListener('click', () => {
      const updated = loadTodos();
      updated.splice(index, 1);
      saveTodos(updated);
      render();
    });

    li.appendChild(left);
    li.appendChild(del);
    list.appendChild(li);
  });
}

addBtn.addEventListener('click', () => {
  const title = input.value.trim();
  if (!title) return;
  const todos = loadTodos();
  todos.push({ title, completed: false });
  saveTodos(todos);
  input.value = '';
  render();
});

input.addEventListener('keydown', (e) => {
  if (e.key === 'Enter') addBtn.click();
});

render();
