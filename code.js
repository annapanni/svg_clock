const hourHand = document.getElementById("hour_hand")
const minHand = document.getElementById("min_hand")
const secHand = document.getElementById("sec_hand")
const styleModeBtn = document.getElementById("styleModeBtn")
const body = document.getElementById("body")
const modeChangers = [styleModeBtn, body]

let darkmode = 0

const style = document.createElement('style');
style.type = 'text/css';

function bounce(obj, fi, deg){
  if (Math.abs(fi-deg)<0.1) return
  let goal = fi + (fi-deg)/3
  const t = setInterval(()=>{
    if (Math.abs(goal-deg)<0.1){
      clearInterval(t)
      bounce(obj, fi, deg)
    }
    obj.setAttribute("transform", `rotate(${deg+(goal-deg)/3}, 100, 100)`)
    deg = deg+(goal-deg)/3
  }, 10)
}

function changeClock(){
  const now = new Date()
  const h = now.getHours()
  const m = now.getMinutes()
  const s = now.getSeconds()

  let fi = 360 * ((3600*h + 60*m + s) / (3600*12))
  hourHand.setAttribute("transform", `rotate(${fi}, 100, 100)`)
  fi = 360 * ((60*m + s) / 3600)
  minHand.setAttribute("transform", `rotate(${fi}, 100, 100)`)
  fi = 360 * (s / 60)
  bounce(secHand, fi, fi-6)
}

const now = new Date()
const h = now.getHours()
const m = now.getMinutes()
const s = now.getSeconds()

let fi = 360 * ((3600*h + 60*m + s) / (3600*12))
hourHand.setAttribute("transform", `rotate(${fi}, 100, 100)`)
fi = 360 * ((60*m + s) / 3600)
minHand.setAttribute("transform", `rotate(${fi}, 100, 100)`)
fi = 360 * (s / 60)
secHand.setAttribute("transform", `rotate(${fi}, 100, 100)`)

setInterval(changeClock, 1000)

styleModeBtn.addEventListener("click", function(e) {
  if (darkmode) {
    e.target.innerText = "Dark Mode"
    for (let i=0; i<modeChangers.length; i++){
      modeChangers[i].classList.remove("dark")
      modeChangers[i].classList.add("light")
    }
    darkmode = 0
  } else {
    e.target.innerText = "Light Mode"
    for (let i=0; i<modeChangers.length; i++){
      modeChangers[i].classList.remove("light")
      modeChangers[i].classList.add("dark")
    }
    darkmode = 1
  }
})
