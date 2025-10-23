import React, { useMemo, useRef, useState, useEffect } from "react";

/** Layal's Royal Link Game 👑 */

const SWATCH = {
  rose:   { base: "#ff5fa2", glow: "#ff94c1" },
  gold:   { base: "#f5c15c", glow: "#ffe29a" },
  violet: { base: "#b784f5", glow: "#d7b7ff" },
  mint:   { base: "#6fd3b8", glow: "#aaf0dd" },
  pearl:  { base: "#d1d5db", glow: "#f3f4f6" },
  ruby:   { base: "#ff4d6d", glow: "#ff8fa3" },
};
const NEON = { pink: "#F535AA", bg: "#1C1522", glow: "rgba(245,53,170,0.6)" };
const LEVELS = [
  {
    size: 5,
    endpoints: [
      { color: "rose",   a: { r: 0, c: 0 }, b: { r: 1, c: 4 } },
      { color: "violet", a: { r: 2, c: 1 }, b: { r: 4, c: 4 } },
      { color: "mint",   a: { r: 3, c: 3 }, b: { r: 4, c: 0 } },
    ],
  },
  
  {
    size: 6,
    endpoints: [
      { color: "rose", a: { r: 0, c: 1 }, b: { r: 2, c: 5 } },
      { color: "gold", a: { r: 1, c: 0 }, b: { r: 4, c: 3 } },
      { color: "mint", a: { r: 5, c: 1 }, b: { r: 3, c: 4 } },
      { color: "ruby", a: { r: 2, c: 2 }, b: { r: 3, c: 1 } },
    ],
  },
  
  {
    size: 7,
    endpoints: [
      { color: "rose",   a: { r: 0, c: 1 }, b: { r: 1, c: 5 } },
      { color: "gold",   a: { r: 1, c: 0 }, b: { r: 4, c: 2 } },
      { color: "mint",   a: { r: 6, c: 1 }, b: { r: 3, c: 4 } },
      { color: "pearl",  a: { r: 2, c: 6 }, b: { r: 5, c: 5 } },
      { color: "violet", a: { r: 1, c: 2 }, b: { r: 5, c: 3 } },
    ],
  },
];

const within = (n, size) => n >= 0 && n < size;
const key = (r, c) => `${r},${c}`;
const dirs = [{ r: -1, c: 0 }, { r: 1, c: 0 }, { r: 0, c: -1 }, { r: 0, c: 1 }];
function neighbors(size, r, c) {
  return dirs.map(d => ({ r: r + d.r, c: c + d.c }))
             .filter(p => within(p.r, size) && within(p.c, size));
}
function titleCase(s) { return s.charAt(0).toUpperCase() + s.slice(1); }
function cellBg(cell) { return cell.color ? `${SWATCH[cell.color].glow}22` : "rgba(18,14,23,0.9)"; }
function NeonButton({ children, variant = "solid", style = {}, ...props }) {
  const base = "px-4 py-2 rounded-xl font-medium transition active:scale-[.98] will-change-transform";
  if (variant === "outline") {
    return (
      <button
        className={`${base} text-white bg-black/30 border backdrop-blur hover:bg-black/50`}
        style={{
          borderColor: NEON.pink, borderWidth: 1,
          boxShadow: `0 0 18px ${NEON.glow}, inset 0 0 12px rgba(255,255,255,0.05)`,
          ...style,
        }}
        {...props}
      >{children}</button>
    );
  }
  return (
    <button
      className={`${base} text-black hover:brightness-110`}
      style={{ background: NEON.pink, boxShadow: `0 0 24px ${NEON.glow}`, ...style }}
      {...props}
    >{children}</button>
  );
}
function LegendPill({ color }) {
  return (
    <div className="px-3 py-1 rounded-full bg-black/40 border border-white/15 text-white flex items-center gap-2 shadow-[0_0_12px_rgba(255,255,255,0.05)]">
      <span className="inline-block w-3 h-3 rounded-full animate-pulse"
            style={{ background: SWATCH[color].base, boxShadow: `0 0 10px ${SWATCH[color].glow}` }} />
      <span className="font-medium">{titleCase(color)}</span>
    </div>
  );
}
function Gem({ color }) {
  const sw = SWATCH[color];
  return (
    <div className="w-3/4 h-3/4 rounded-2xl grid place-items-center transition-transform duration-200"
         style={{
           background: `radial-gradient(circle at 30% 30%, ${sw.glow}, ${sw.base})`,
           boxShadow: `0 0 16px ${sw.glow}, inset 0 0 10px #ffffffaa`,
           transform: "translateZ(0)",
         }}>
      <div className="w-1/3 h-1/3 rounded-xl bg-white/70" />
    </div>
  );
}

export default function App() {
  const [levelIdx, setLevelIdx] = useState(0);
  const level = LEVELS[levelIdx];

  const initialBoard = useMemo(() => {
    const grid = Array.from({ length: level.size }, (_, r) =>
      Array.from({ length: level.size }, (_, c) => ({ r, c }))
    );
    for (const e of level.endpoints) {
      const A = grid[e.a.r][e.a.c], B = grid[e.b.r][e.b.c];
      A.color = e.color; A.endpoint = true;
      B.color = e.color; B.endpoint = true;
    }
    return grid;
  }, [level]);

  const [board, setBoard]   = useState(initialBoard);
  const [paths, setPaths]   = useState(() => { const p = {}; for (const e of level.endpoints) p[e.color] = []; return p; });
  const [active, setActive] = useState(null); // { color, id } | null
  const [dragging, setDragging] = useState(false);
  const history = useRef([]);
  const [fadeKey, setFadeKey] = useState(() => `lvl-${Date.now()}`);

  useEffect(() => {
    setBoard(initialBoard);
    const p = {}; for (const e of level.endpoints) p[e.color] = [];
    setPaths(p); setActive(null); history.current = [];
    setFadeKey(`lvl-${Date.now()}`);
  }, [initialBoard, level]);

  function cloneBoard(src) { return src.map(row => row.map(c => ({ ...c }))); }
  function pushHistory() { history.current.push({ board: cloneBoard(board), paths: JSON.parse(JSON.stringify(paths)) }); }
  function undo() { const prev = history.current.pop(); if (!prev) return; setBoard(prev.board); setPaths(prev.paths); setActive(null); }
  function cellAt(r, c) { return board[r][c]; }
  function canPaint(to, color) {
    if (to.endpoint && to.color !== color) return false;
    if (!to.endpoint && to.color && to.color !== color) return false;
    return true;
  }

  function startPath(r, c) {
    const start = cellAt(r, c);
    if (!start.color) return;
    const color = start.color;
    pushHistory();

    const newBoard = cloneBoard(board);
    for (let i = 0; i < level.size; i++) for (let j = 0; j < level.size; j++) {
      const cell = newBoard[i][j];
      if (!cell.endpoint && cell.color === color) { cell.color = undefined; cell.pathId = undefined; }
    }

    const newPaths = { ...paths, [color]: [{ r, c }] };
    const id = `${color}-${Date.now()}`;
    newBoard[r][c].pathId = id;
    setBoard(newBoard); setPaths(newPaths);
    setActive({ color, id }); setDragging(true);
  }

  function extendPath(r, c) {
    if (!active) return;
    const { color, id } = active;
    const last = paths[color][paths[color].length - 1];
    if (!last) return;
    const dr = Math.abs(r - last.r), dc = Math.abs(c - last.c);
    if (dr + dc !== 1) return;
    const target = cellAt(r, c);
    if (!canPaint(target, color)) return;

    pushHistory();

    const same = !target.endpoint && target.color === color && target.pathId === id;
    const newBoard = cloneBoard(board);
    const newPaths = { ...paths, [color]: [...paths[color]] };

    if (same) {
      const idx = newPaths[color].findIndex(p => p.r === r && p.c === c);
      if (idx >= 0) {
        for (let k = idx + 1; k < newPaths[color].length; k++) {
          const p = newPaths[color][k];
          const cell = newBoard[p.r][p.c];
          if (!cell.endpoint) { cell.color = undefined; cell.pathId = undefined; }
        }
        newPaths[color] = newPaths[color].slice(0, idx + 1);
      }
      setBoard(newBoard); setPaths(newPaths);
      return;
    }

    newBoard[r][c].color = color; newBoard[r][c].pathId = id;
    newPaths[color].push({ r, c });
    setBoard(newBoard); setPaths(newPaths);
  }

  function endDrag() { setDragging(false); setActive(null); }
  function handlePointerDown(e, r, c) { e.preventDefault(); startPath(r, c); }
  function handlePointerEnter(e, r, c) { if (!dragging) return; extendPath(r, c); }
  function clearColor(color) {
    pushHistory();
    const newBoard = cloneBoard(board);
    for (let i = 0; i < level.size; i++) for (let j = 0; j < level.size; j++) {
      const cell = newBoard[i][j];
      if (!cell.endpoint && cell.color === color) { cell.color = undefined; cell.pathId = undefined; }
    }
    const newPaths = { ...paths, [color]: [] };
    setBoard(newBoard); setPaths(newPaths);
  }
  function resetBoard() {
    pushHistory();
    const fresh = initialBoard.map(row => row.map(c => ({ ...c })));
    setBoard(fresh);
    const p = {}; for (const e of level.endpoints) p[e.color] = [];
    setPaths(p); setActive(null);
  }

  // WIN RULE: connect each pair (you don't need to fill all squares)
  const solved = useMemo(() => {
    for (const e of level.endpoints) {
      const p = paths[e.color];
      if (!p || p.length < 2) return false;
      const hasA = p.some(q => q.r === e.a.r && q.c === e.a.c);
      const hasB = p.some(q => q.r === e.b.r && q.c === e.b.c);
      if (!hasA || !hasB) return false;
    }
    return true;
  }, [paths, level]);

  useEffect(() => {
    if (!solved) return;
    const msgs = [
      "yeyy — you passed my Level 1! 💖",
      "you’re a smart software! Level 2 cleared 🤝",
      "yeyyy — you completed Layal Link Game! 👑",
    ];
    const msg = msgs[levelIdx] || "Nice!";
    const t = setTimeout(() => {
      alert(msg);
      if (levelIdx < LEVELS.length - 1) setLevelIdx(i => i + 1);
    }, 250);
    return () => clearTimeout(t);
  }, [solved, levelIdx]);

  const stageStyle = {
    minHeight: "100vh",
    backgroundColor: NEON.bg,
    backgroundImage: `
      radial-gradient(1200px 400px at 50% -100px, ${NEON.pink}55, transparent 60%),
      radial-gradient(800px 300px at 50% 100%, ${NEON.pink}33, transparent 70%)
    `,
    animation: "bgFloat 16s ease-in-out infinite",
  };

  function onTouchMove(e) {
    if (!dragging) return;
    const t = e.touches[0];
    const el = document.elementFromPoint(t.clientX, t.clientY);
    if (!el) return;
    const r = el.getAttribute("data-r");
    const c = el.getAttribute("data-c");
    if (r && c) extendPath(parseInt(r), parseInt(c));
  }

  return (
    <div style={stageStyle} onMouseUp={endDrag} onTouchEnd={endDrag} onTouchMove={onTouchMove}>
      <div key={fadeKey} className="w-full max-w-4xl mx-auto p-4 md:p-6 animate-[fadeIn_.35s_ease]">
        {/* Header */}
        <div className="mb-4 flex items-center justify-between">
          <div className="text-white">
            <h1 className="text-2xl md:text-3xl font-bold drop-shadow">Layal’s Royal Link Game 👑</h1>
            <p className="opacity-90">
              Connect matching glam gems. Paths can’t cross. <b>You don’t need to cover every square.</b>
            </p>
          </div>
          <div className="flex gap-2">
            <NeonButton variant="outline" onClick={() => setLevelIdx((i) => (i - 1 + LEVELS.length) % LEVELS.length)}>◀ Level</NeonButton>
            <span className="px-3 py-2 rounded-xl bg-white/10 text-white/90 font-semibold border border-white/20 backdrop-blur">
              {`Level ${levelIdx + 1}/${LEVELS.length}`}
            </span>
            <NeonButton variant="outline" onClick={() => setLevelIdx((i) => (i + 1) % LEVELS.length)}>Level ▶</NeonButton>
            <NeonButton onClick={() => { if (levelIdx < LEVELS.length - 1) setLevelIdx(i => i + 1); }}>Skip ▶</NeonButton>
          </div>
        </div>

        {/* Board */}
        <div className="rounded-3xl p-4 md:p-6 bg-black/50 backdrop-blur border border-pink-500/40" style={{ boxShadow: `0 0 40px ${NEON.glow}` }}>
          {/* legend + actions */}
          <div className="flex items-center justify-between mb-4 gap-3 flex-wrap">
            <div className="flex gap-2 flex-wrap items-center">
              {level.endpoints.map(e => <LegendPill key={e.color} color={e.color} />)}
            </div>
            <div className="flex gap-2">
              <NeonButton onClick={undo}>Undo</NeonButton>
              <NeonButton onClick={hint}>Hint</NeonButton>
              <NeonButton onClick={resetBoard}>Reset</NeonButton>
            </div>
          </div>

          {/* grid */}
          <div className="relative mx-auto" style={{ width: Math.min(560, level.size * 72) }}>
            <div className="grid select-none rounded-2xl overflow-hidden"
                 style={{ gridTemplateColumns: `repeat(${level.size},1fr)`, gridTemplateRows: `repeat(${level.size},1fr)` }}>
              {board.map((row, r) => row.map((cell, c) => (
                <div key={key(r, c)} data-r={r} data-c={c}
                     onMouseDown={(e) => handlePointerDown(e, r, c)}
                     onMouseEnter={(e) => handlePointerEnter(e, r, c)}
                     onTouchStart={(e) => handlePointerDown(e, r, c)}
                     className="aspect-square border border-pink-500/20 flex items-center justify-center relative transition-[background,transform,box-shadow] duration-150 hover:scale-[1.02] active:scale-[.98] will-change-transform"
                     style={{ background: cellBg(cell), boxShadow: cell.color ? `inset 0 0 18px ${SWATCH[cell.color].glow}` : "none" }}>
                  {cell.endpoint && <Gem color={cell.color} />}
                  {!cell.endpoint && cell.color && (
                    <div className="w-2/3 h-2/3 rounded-xl transition-all duration-150"
                         style={{
                           background: `radial-gradient(circle at 30% 30%, ${SWATCH[cell.color].glow}, ${SWATCH[cell.color].base})`,
                           boxShadow: `0 0 12px ${SWATCH[cell.color].glow}`,
                           transform: "translateZ(0)",
                         }} />
                  )}
                </div>
              )))}
            </div>
          </div>

          {/* per-color clear */}
          <div className="mt-4 flex flex-wrap gap-2">
            {level.endpoints.map((e) => (
              <NeonButton key={e.color} variant="outline" onClick={() => clearColor(e.color)}>
                Clear <span style={{ color: SWATCH[e.color].base }} className="font-semibold ml-1">{titleCase(e.color)}</span>
              </NeonButton>
            ))}
          </div>
        </div>

        <div className="text-white/80 text-sm mt-3">
          <p><b>Rules:</b> Connect each color’s two gems. Paths can’t cross. <b>You don’t need to fill the whole board.</b></p>
          <p className="mt-1"><b>Tip:</b> Corners & tight corridors first; look for forced moves.</p>
        </div>
      </div>

      <style>{`
        @keyframes fadeIn {
          from { opacity: 0; transform: translateY(8px); }
          to   { opacity: 1; transform: translateY(0); }
        }
        @keyframes bgFloat {
          0%,100% { background-position: 0px 0px, 0px 0px; }
          50%     { background-position: 0px -10px, 0px 10px; }
        }
      `}</style>
    </div>
  );

 
  function hint() {
    for (const e of level.endpoints) {
      const tips = [e.a, e.b];
      for (const t of tips) {
        const neigh = neighbors(level.size, t.r, t.c);
        const free = neigh.filter((n) => {
          const cell = cellAt(n.r, n.c);
          if (!cell.color) return true;
          if (!cell.endpoint && cell.color === e.color) return true;
          return false;
        });
        if (free.length === 1) {
          startPath(t.r, t.c);
          setTimeout(() => {
            extendPath(free[0].r, free[0].c);
            endDrag();
          }, 0);
          return;
        }
      }
    }
    alert("No obvious forced move. Try a shorter color or clear a conflicting path.");
  }
}
